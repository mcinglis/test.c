// tests/test.c

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


#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <test.h> // Test, Assertions, TEST*, test*, assertion*

#include <_common.h> // NELEM


// ----------
// Example functions and data for use in testing.
// ----------

static Assertions * func_1( void ) { return assertions( 1 == 1, 10 > 3 ); }
static Assertions * func_2( void ) { return assertions_empty(); }

static Assertions * func_fail_1( void ) { return assertions( 2 == 2, 1 < 1 ); }
static Assertions * func_fail_2( void ) { return assertions( false ); }

// This lets us test that `TEST_ARRAY` works when its arguments aren't
// just function names.
static test_fn func_gen( int const x, int const y ) { return func_1; }

// None of these tests should be equal.
static Test const tests[] = {
    { .func = func_1,      .name = "" },
    { .func = func_1,      .name = "foo" },
    { .func = func_1,      .name = "example name" },
    { .func = func_2,      .name = "" },
    { .func = func_2,      .name = "foo" },
    { .func = func_2,      .name = "example name" },
    { .func = func_fail_1, .name = "" },
    { .func = func_fail_1, .name = "foo" },
    { .func = func_fail_1, .name = "example name" },
};

// ----------
// End of example functions and data.
// ----------


static
FILE * open_output( void )
{
    return fopen( "/dev/null", "w" );
}


static
Assertions * test_eq__works( void )
{
    Assertions * const as = assertions_empty();
    for ( size_t i = 0; i < NELEM( tests ); i += 1 ) {
        for ( size_t j = 0; j < NELEM( tests ); j += 1 ) {
            bool const eq_ij = test_eq( tests[ i ], tests[ j ] );
            bool const eq_ji = test_eq( tests[ j ], tests[ i ] );
            if ( j == i ) {
                assertions_add( as, eq_ij, i, j );
                assertions_add( as, eq_ji, i, j );
            } else {
                assertions_add( as, !eq_ij, i, j );
                assertions_add( as, !eq_ji, i, j );
            }
        }
    }
    return as;
}


static
Assertions * TEST_ARRAY__gives_right_tests( void )
{
    Test const ts[] = TEST_ARRAY(
        func_1,
        func_gen( 1, 2 ),
        func_2
    );
    return assertions(
        test_eq( ts[ 0 ], ( Test ){
            .func = func_1,
            .name = "func_1"
        } ),
        test_eq( ts[ 1 ], ( Test ){
            .func = func_gen( 1, 2 ),
            .name = "func_gen( 1, 2 )"
        } ),
        test_eq( ts[ 2 ], ( Test ){
            .func = func_2,
            .name = "func_2"
        } ),
        ts[ 3 ].func == NULL
    );
}


static
Assertions * tests_run__no_fails( void )
{
    // Given:
    char const name[] = "no fails";
    Test const ts[] = TEST_ARRAY( func_1, func_2, func_gen( 1, 2 ) );
    FILE * const output = open_output();

    // When:
    int const fails = tests_run( .name = name, .tests = ts, .file = output );
    fclose( output );

    // Then:
    return assertions( fails == 0 );
}


static
Assertions * tests_run__some_fails( void )
{
    // Given:
    char const name[] = "some fails";
    Test const ts[] = TEST_ARRAY( func_1, func_fail_1, func_2,
                                  func_fail_2, func_1, func_fail_2 );
    FILE * const output = open_output();

    // When:
    int const fails = tests_run( .name = name, .tests = ts, .file = output );
    fclose( output );

    // Then:
    return assertions( fails == 3 );
}


static
Assertions * tests_run__all_fails( void )
{
    // Given:
    char const name[] = "all fails";
    Test const ts[] = TEST_ARRAY( func_fail_2, func_fail_1 );
    FILE * const output = open_output();

    // When:
    int const fails = tests_run( .name = name, .tests = ts, .file = output );
    fclose( output );

    // Then:
    return assertions( fails == 2 );
}


static
Assertions * tests_return_val__works( void )
{
    return assertions(
        tests_return_val( 0 ) == 0,
        tests_return_val( 0, 0 ) == 0,
        tests_return_val( 3 ) == 1,
        tests_return_val( 0, 1 ) == 1,
        tests_return_val( 0, 0, 2 ) == 1,
        tests_return_val( 0, 1, 0 ) == 1,
        tests_return_val( 4, 2, 1 ) == 1
    );
}


Test const test_tests[] = TEST_ARRAY(
    test_eq__works,
    TEST_ARRAY__gives_right_tests,
    tests_run__no_fails,
    tests_run__some_fails,
    tests_run__all_fails,
    tests_return_val__works
);

