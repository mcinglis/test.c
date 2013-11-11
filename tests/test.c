// tests/test.c
// Tests the functions exported by `test.h`.

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
#include <unistd.h>

#include <test.h>

#include "common.h" // NELEM


// ----------
// Example functions and data for use in testing.
// ----------

static TestAssertion * ex_func_1( void * const d ) { return NULL; }
static TestAssertion * ex_func_2( void * const d ) { return NULL; }

// This lets us test that the `TESTS` and `TESTS_FIX` macros work
// correctly when their arguments aren't just a function name.
static test_function ex_func_gen( int const x, int const y )
    { return ex_func_1; }

static TestAssertion * ex_func_fail_1( void * const d )
    { return test_assert( false ); }
static TestAssertion * ex_func_fail_2( void * const d )
    { return test_assert( false ); }

static void * ex_before_1( void ) { return NULL; }
static void * ex_before_2( void ) { return NULL; }

static void ex_after_1( void * const d ) {}
static void ex_after_2( void * const d ) {}

// None of these tests should be equal.
static Test const ex_tests[] = {

    // `test_is_end__is_correct()` requires this to be first, so you
    // need to iterate over this array with `NELEM`.
    TESTS_END,

    // No fixture functions.
    { .func = ex_func_1, .name = "" },
    { .func = ex_func_1, .name = "foo" },
    { .func = ex_func_1, .name = "example name" },
    { .func = ex_func_2, .name = "" },
    { .func = ex_func_2, .name = "foo" },
    { .func = ex_func_2, .name = "example name" },

    // Only a `before` fixture function.
    { .func = ex_func_1, .name = "",             .before = ex_before_1 },
    { .func = ex_func_1, .name = "foo",          .before = ex_before_1 },
    { .func = ex_func_1, .name = "example name", .before = ex_before_1 },
    { .func = ex_func_2, .name = "",             .before = ex_before_1 },
    { .func = ex_func_2, .name = "foo",          .before = ex_before_1 },
    { .func = ex_func_2, .name = "example name", .before = ex_before_1 },
    { .func = ex_func_1, .name = "",             .before = ex_before_2 },
    { .func = ex_func_1, .name = "foo",          .before = ex_before_2 },
    { .func = ex_func_1, .name = "example name", .before = ex_before_2 },
    { .func = ex_func_2, .name = "",             .before = ex_before_2 },
    { .func = ex_func_2, .name = "foo",          .before = ex_before_2 },
    { .func = ex_func_2, .name = "example name", .before = ex_before_2 },

    // Only an `after` fixture function.
    { .func = ex_func_1, .name = "",             .after = ex_after_1 },
    { .func = ex_func_1, .name = "foo",          .after = ex_after_1 },
    { .func = ex_func_1, .name = "example name", .after = ex_after_1 },
    { .func = ex_func_2, .name = "",             .after = ex_after_1 },
    { .func = ex_func_2, .name = "foo",          .after = ex_after_1 },
    { .func = ex_func_2, .name = "example name", .after = ex_after_1 },
    { .func = ex_func_1, .name = "",             .after = ex_after_2 },
    { .func = ex_func_1, .name = "foo",          .after = ex_after_2 },
    { .func = ex_func_1, .name = "example name", .after = ex_after_2 },
    { .func = ex_func_2, .name = "",             .after = ex_after_2 },
    { .func = ex_func_2, .name = "foo",          .after = ex_after_2 },
    { .func = ex_func_2, .name = "example name", .after = ex_after_2 },

    // Both a `before` and `after` fixture functions.
    { .func = ex_func_1, .name = "",             .before = ex_before_1,
                                                 .after  = ex_after_1 },
    { .func = ex_func_1, .name = "foo",          .before = ex_before_1,
                                                 .after  = ex_after_1 },
    { .func = ex_func_1, .name = "example name", .before = ex_before_1,
                                                 .after  = ex_after_1 },
    { .func = ex_func_2, .name = "",             .before = ex_before_1,
                                                 .after  = ex_after_1 },
    { .func = ex_func_2, .name = "foo",          .before = ex_before_1,
                                                 .after  = ex_after_1 },
    { .func = ex_func_2, .name = "example name", .before = ex_before_1,
                                                 .after  = ex_after_1 },
    { .func = ex_func_1, .name = "",             .before = ex_before_2,
                                                 .after  = ex_after_1 },
    { .func = ex_func_1, .name = "foo",          .before = ex_before_2,
                                                 .after  = ex_after_1 },
    { .func = ex_func_1, .name = "example name", .before = ex_before_2,
                                                 .after  = ex_after_1 },
    { .func = ex_func_2, .name = "",             .before = ex_before_2,
                                                 .after  = ex_after_1 },
    { .func = ex_func_2, .name = "foo",          .before = ex_before_2,
                                                 .after  = ex_after_1 },
    { .func = ex_func_2, .name = "example name", .before = ex_before_2,
                                                 .after  = ex_after_1 },
    { .func = ex_func_1, .name = "",             .before = ex_before_1,
                                                 .after  = ex_after_2 },
    { .func = ex_func_1, .name = "foo",          .before = ex_before_1,
                                                 .after  = ex_after_2 },
    { .func = ex_func_1, .name = "example name", .before = ex_before_1,
                                                 .after  = ex_after_2 },
    { .func = ex_func_2, .name = "",             .before = ex_before_1,
                                                 .after  = ex_after_2 },
    { .func = ex_func_2, .name = "foo",          .before = ex_before_1,
                                                 .after  = ex_after_2 },
    { .func = ex_func_2, .name = "example name", .before = ex_before_1,
                                                 .after  = ex_after_2 },
    { .func = ex_func_1, .name = "",             .before = ex_before_2,
                                                 .after  = ex_after_2 },
    { .func = ex_func_1, .name = "foo",          .before = ex_before_2,
                                                 .after  = ex_after_2 },
    { .func = ex_func_1, .name = "example name", .before = ex_before_2,
                                                 .after  = ex_after_2 },
    { .func = ex_func_2, .name = "",             .before = ex_before_2,
                                                 .after  = ex_after_2 },
    { .func = ex_func_2, .name = "foo",          .before = ex_before_2,
                                                 .after  = ex_after_2 },
    { .func = ex_func_2, .name = "example name", .before = ex_before_2,
                                                 .after  = ex_after_2 }
};

// ----------
// End of example functions and data.
// ----------


static
void * before_each( void )
{
    FILE * const output = fopen( "/dev/null", "w" );
    return output;
}


static
void after_each( void * const data )
{
    FILE * const output = data;
    fclose( output );
}


static
TestAssertion * test_eq__is_correct( void * const data )
{
    for ( size_t i = 0; i < NELEM( ex_tests ); i += 1 ) {
        for ( size_t j = 0; j < NELEM( ex_tests ); j += 1 ) {
            bool const both_equal = test_eq( ex_tests[ i ], ex_tests[ j ] )
                                 && test_eq( ex_tests[ j ], ex_tests[ i ] );
            if ( j == i ) {
                TEST_REQUIRE( both_equal, i, j );
            } else {
                TEST_REQUIRE( !both_equal, i, j );
            }
        }
    }
    return NULL;
}


static
TestAssertion * test_is_end__is_correct( void * const data )
{
    for ( size_t i = 1; i < NELEM( ex_tests ); i += 1 ) {
        TEST_REQUIRE( !test_is_end( ex_tests[ i ] ), i );
    }
    return test_assert( test_is_end( ex_tests[ 0 ] ) );
}


static
TestAssertion * TESTS__gives_right_tests( void * const data )
{
    Test const tests[] = TESTS( ex_func_1,
                                ex_func_gen( 1, 2 ),
                                ex_func_2 );
    return test_assert(
        test_eq( tests[ 0 ], ( Test ){
            .name = "ex_func_1", .func = ex_func_1
        } ),
        test_eq( tests[ 1 ], ( Test ){
            .name = "ex_func_gen( 1, 2 )", .func = ex_func_gen( 1, 2 )
        } ),
        test_eq( tests[ 2 ], ( Test ){
            .name = "ex_func_2", .func = ex_func_2
        } ),
        test_is_end( tests[ 3 ] )
    );
}


static
TestAssertion * TESTS_FIX__gives_right_tests( void * const data )
{
    Test const tests[] = TESTS_FIX( ex_before_1, ex_after_1,
        ex_func_1,
        ex_func_2,
        ex_func_gen( 3, 2 )
    );
    return test_assert(
        test_eq( tests[ 0 ], ( Test ){
            .name = "ex_func_1",
            .func = ex_func_1,
            .before = ex_before_1,
            .after = ex_after_1
        } ),
        test_eq( tests[ 1 ], ( Test ){
            .name = "ex_func_2",
            .func = ex_func_2,
            .before = ex_before_1,
            .after = ex_after_1
        } ),
        test_eq( tests[ 2 ], ( Test ){
            .name = "ex_func_gen( 3, 2 )",
            .func = ex_func_gen( 3, 2 ),
            .before = ex_before_1,
            .after = ex_after_1
        } ),
        test_is_end( tests[ 3 ] )
    );
}


static
TestAssertion * tests_run__gives_right_results( void * const data )
{
    FILE * const output = data;
    TestResults const tr1 = tests_run(
        .name = "no fails",
        .tests = ( Test[] ) TESTS( ex_func_1, ex_func_2 ),
        .file = output
    );
    TestResults const tr2 = tests_run(
        .name = "all fails",
        .tests = ( Test[] ) TESTS( ex_func_fail_1, ex_func_fail_2 ),
        .file = output
    );
    TestResults const tr3 = tests_run(
        .name = "mixed",
        .tests = ( Test[] ) TESTS( ex_func_1, ex_func_fail_1, ex_func_2,
                                   ex_func_fail_2, ex_func_1 ),
        .file = output
    );
    return test_assert(
        tr1.passed == 2, tr1.failed == 0,
        tr2.passed == 0, tr2.failed == 2,
        tr3.passed == 3, tr3.failed == 2
    );
}


static
TestAssertion * tests_return_val__gives_0_when_all_pass( void * const data )
{
    FILE * const output = data;
    int v = tests_return_val(
        tests_run(
            .name = "all pass",
            .tests = ( Test[] ) TESTS( ex_func_1, ex_func_2,
                                       ex_func_gen( 1, 2 ) ),
            .file = output
        ),
        tests_run(
            .name = "also all pass",
            .tests = ( Test[] ) TESTS( ex_func_gen( 3, 4 ) ),
            .file = output
        )
    );
    return test_assert( v == 0 );
}


static
TestAssertion * tests_return_val__gives_1_when_some_fail( void * const data )
{
    FILE * const output = data;
    int v = tests_return_val(
        tests_run(
            .name = "all pass",
            .tests = ( Test[] ) TESTS( ex_func_1, ex_func_2,
                                       ex_func_gen( 1, 2 ) ),
            .file = output
        ),
        tests_run(
            .name = "one fail",
            .tests = ( Test[] ) TESTS( ex_func_gen( 3, 4 ), ex_func_fail_1 ),
            .file = output
        )
    );
    return test_assert( v == 1 );
}


Test const test_tests[] = TESTS_FIX( before_each, after_each,
    test_eq__is_correct,
    test_is_end__is_correct,
    TESTS__gives_right_tests,
    TESTS_FIX__gives_right_tests,
    tests_run__gives_right_results,
    tests_return_val__gives_0_when_all_pass,
    tests_return_val__gives_1_when_some_fail
);

