// test.c

// Copyright (C) 2013  Malcolm Inglis <http://minglis.id.au/>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.


#include "test.h"       // Test, TestAssertion, TestResults

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "assertion-private.h"


bool test_eq( Test const t1, Test const t2 )
{
    return t1.func == t2.func
        && t1.before == t2.before
        && t1.after == t2.after
        && string_eq( t1.name, t2.name );
}


bool test_is_end( Test const t )
{
    return test_eq( t, ( Test ) TESTS_END );
}


static
TestAssertion * test_gen_assertions( Test const test )
{
    void * const data = ( test.before == NULL ) ? NULL : test.before();
    TestAssertion * const assertions = test.func( data );
    if ( test.after != NULL ) {
        test.after( data );
    }
    return assertions;
}


static
bool test_run( Test const test, FILE * file, char const * const indent )
{
    assert( file != NULL );
    assert( test.func != NULL );
    assert( indent != NULL );

    bool pass = true;
    TestAssertion * const as = test_gen_assertions( test );
    if ( as != NULL ) {
        for ( size_t i = 0; !test_assertion_is_end( as[ i ] ); i += 1 ) {
            if ( as[ i ].result == false ) {
                // If this is the first false assertion we've seen,
                // print a line saying that the test has failed.
                if ( pass == true ) {
                    fprintf( file, "%s", indent );
                    fprintf( file, "fail:  %s\n", test.name );
                }
                fprintf( file, "%s%s", indent, indent );
                test_assertion_print( as[ i ], file, indent );
                pass = false;
            }
        }
        test_assertions_free( as );
    }
    if ( pass == true ) {
        fprintf( file, "%s", indent );
        fprintf( file, "pass:  %s\n", test.name );
    }
    return pass;
}


TestResults tests_run_( struct tests_run_options o )
{
    assert( o.name != NULL );
    assert( o.tests != NULL );
    if ( o.file == NULL ) {
        o.file = stdout;
    }

    fprintf( o.file, "Running %s tests...\n", o.name );
    TestResults results = { .passed = 0, .failed = 0 };
    for ( size_t i = 0; !test_is_end( o.tests[ i ] ); i += 1 ) {
        bool passed = test_run( o.tests[ i ], o.file, "    " );
        if ( passed ) {
            results.passed += 1;
        } else {
            results.failed += 1;
        }
    }
    fprintf( o.file, "Finished %s tests: %d passed, and %d failed.\n",
             o.name, results.passed, results.failed );
    return results;
}


int tests_return_val_( TestResults const * rs )
{
    assert( rs != NULL );

    // `{ .failed = -1 }` is the terminating value.
    for ( size_t i = 0; rs[ i ].failed != -1; i += 1 ) {
        if ( rs[ i ].failed > 0 ) {
            return 1;
        }
    }
    return 0;
}

