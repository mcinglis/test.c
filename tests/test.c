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


#include <stdlib.h>

#include <test.h>
// Test, TestAssertion, TESTS, test_assertion, test_assertion_eq


static TestAssertion * dummy_func_1( void * const d ) { return NULL; }
static TestAssertion * dummy_func_2( void * const d ) { return NULL; }

static void * dummy_before_1( void ) { return NULL; }
static void * dummy_before_2( void ) { return NULL; }

static void dummy_after_1( void * const d ) {}
static void dummy_after_2( void * const d ) {}


static
TestAssertion * test_eq__is_correct( void * const data )
{
    // None of these tests should be equal.
    Test const tests[] = {
        { 0 },
        { .name = "foo" },
        { .name = "bar" },
        { .func = dummy_func_1 },
        { .func = dummy_func_2 },
        { .name = "foo",
          .func = dummy_func_1 },
        { .name = "foo",
          .func = dummy_func_2 },
        { .name = "bar",
          .func = dummy_func_1 },
        { .name = "foo",
          .func = dummy_func_1,
          .before = dummy_before_1 },
        { .name = "foo",
          .func = dummy_func_1,
          .before = dummy_before_2 },
        { .name = "foo",
          .func = dummy_func_2,
          .before = dummy_before_1 },
        { .name = "bar",
          .func = dummy_func_1,
          .before = dummy_before_1 },
        { .name = "foo",
          .func = dummy_func_1,
          .after = dummy_after_1 },
        { .name = "foo",
          .func = dummy_func_1,
          .after = dummy_after_2 },
        { .name = "foo",
          .func = dummy_func_2,
          .after = dummy_after_1 },
        { .name = "bar",
          .func = dummy_func_1,
          .after = dummy_after_1 },
        TESTS_END
    };
    for ( int i = 0; test_is_end( tests[ i ] ); i += 1 ) {
        for ( int j = 0; test_is_end( tests[ j ] ); j += 1 ) {
            if ( j == i ) {
                TEST_REQUIRE( test_eq( tests[ i ], tests[ j ] ), i );
            } else {
                TEST_REQUIRE( !test_eq( tests[ i ], tests[ j ] ), j );
            }
        }
    }
    return NULL;
}


static
TestAssertion * TESTS__gives_right_tests( void * const data )
{
    // We use our own test functions because they have the right
    // signature, and that's all we care about.
    Test const tests[] = TESTS( dummy_func_1, dummy_func_2 );
    return test_assert(
        test_eq( tests[ 0 ], ( Test ){
            .name = "dummy_func_1", .func = dummy_func_1
        } ),
        test_eq( tests[ 1 ], ( Test ){
            .name = "dummy_func_2", .func = dummy_func_2
        } ),
        test_eq( tests[ 2 ], ( Test ) TESTS_END )
    );
}


static
TestAssertion * TESTS_FIX__gives_right_tests( void * const data )
{
    // We use our own test functions because they have the right
    // signature, and that's all we care about.
    Test const tests[] = TESTS_FIX( before_each, after_each,
        TESTS__gives_right_tests,
        TESTS_FIX__gives_right_tests
    );
    return test_assert(
        test_eq( tests[ 0 ], ( Test ){
            .name = "TESTS__gives_right_tests",
            .func = TESTS__gives_right_tests
        } ),
        test_eq( tests[ 1 ], ( Test ){
            .name = "TEST_REQUIRE__returns_only_when_false",
            .func = TEST_REQUIRE__returns_only_when_false
        } ),
        test_eq( tests[ 2 ], ( Test ){ .func = NULL } )
    );
}


Test const test_tests[] = TESTS(
    test_eq__is_correct,
    TESTS__gives_right_tests,
    TESTS_FIX__gives_right_tests
);

