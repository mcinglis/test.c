// test.c

// Copyright (C) 2013  Malcolm Inglis
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

#define INDENT "    "   // Four spaces.


// Counts how many assertions there are in the given array up to and
// including the terminating assertion with a null expression field.
size_t test_assert_size( TestAssertion const * const assertions )
{
    int len = 0;
    for ( int i = 0; assertions[ i ].expr != NULL; i += 1 ) {
        len += 1;
    }
    return len;
}


TestAssertion * test_assertions_alloc( TestAssertion const * const assertions )
{
    size_t const nbytes = test_assert_size( assertions )
                          * sizeof( TestAssertion );
    TestAssertion * const copy = malloc( nbytes );
    memcpy( copy, assertions, nbytes );
    return copy;
}


static
bool test_run( Test const test )
{
    assert( test.func != NULL );
    TestAssertion * const assertions = test.func();
    bool pass = true;
    if ( assertions != NULL ) {
        for ( int i = 0; assertions[ i ].expr != NULL; i += 1 ) {
            TestAssertion const a = assertions[ i ];
            if ( a.result == false ) {
                if ( pass == true ) {
                    printf( INDENT "fail: %s\n", test.name );
                }
                printf( INDENT INDENT "false: %s", a.expr );
                if ( a.id_expr != NULL ) {
                    printf( INDENT "(%s = %d)", a.id_expr, a.id );
                }
                printf( "\n" );
                pass = false;
            }
        }
        free( assertions );
    }
    if ( pass == true ) {
        printf( INDENT "pass: %s\n", test.name );
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

