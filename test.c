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


#define INDENT "    "


static
bool string_eq( char const * const s1, char const * const s2 )
{
    return s1 == s2
        || ( s1 != NULL
          && s2 != NULL
          && strcmp( s1, s2 ) == 0 );
}


bool test_assertion_eq( TestAssertion const a1, TestAssertion const a2 )
{
    return a1.result == a2.result
        && a1.id == a2.id
        && string_eq( a1.expr, a2.expr )
        && string_eq( a1.id_expr, a2.id_expr );
}


static
size_t test_assertions_size( TestAssertion const * const assertions )
{
    int len = 0;
    for ( int i = 0; assertions[ i ].expr != NULL; i += 1 ) {
        len += 1;
    }
    return len + 1;
}


TestAssertion * test_assertions_alloc( TestAssertion const * const assertions )
{
    size_t const nbytes = test_assertions_size( assertions )
                          * sizeof( TestAssertion );
    TestAssertion * const copy = malloc( nbytes );
    memcpy( copy, assertions, nbytes );
    return copy;
}


bool test_eq( Test const t1, Test const t2 )
{
    return t1.func == t2.func
        && t1.before == t2.before
        && t1.after == t2.after
        && string_eq( t1.name, t2.name );
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
bool test_run( Test const test )
{
    assert( test.func != NULL && test.name != NULL );
    TestAssertion * const assertions = test_gen_assertions( test );
    bool pass = true;
    if ( assertions != NULL ) {
        for ( int i = 0; assertions[ i ].expr != NULL; i += 1 ) {
            TestAssertion const a = assertions[ i ];
            if ( a.result == false ) {
                // If this is the first false assertion we've seen:
                if ( pass == true ) {
                    printf( INDENT "fail:  %s\n", test.name );
                }
                printf( INDENT INDENT "false" );
                if ( a.id_expr != NULL ) {
                    printf( " for %s = %d", a.id_expr, a.id );
                }
                printf( ":  %s\n", a.expr );
                pass = false;
            }
        }
        free( assertions );
    }
    if ( pass == true ) {
        printf( INDENT "pass:  %s\n", test.name );
    }
    return pass;
}


TestResults tests_run( char const * const name,
                       Test const * const tests )
{
    printf( "Running %s tests...\n", name );
    TestResults results = { .passed = 0, .failed = 0 };
    for ( int i = 0; tests[ i ].func != NULL; i += 1 ) {
        bool passed = test_run( tests[ i ] );
        if ( passed ) {
            results.passed += 1;
        } else {
            results.failed += 1;
        }
    }
    printf( "Finished %s tests: %d passed, and %d failed.\n",
            name, results.passed, results.failed );
    return results;
}

