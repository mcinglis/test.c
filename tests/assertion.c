// tests/assertion.c

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
#include <string.h>

#include <test.h>

#include "common.h" // NELEM


#define FREE_AFTER_SIZE 5


// ----------
// Example functions and data for use in testing.
// ----------

// Used as an example of a function call as a boolean expression.
static bool numbers_eq( int x, int y, int z ) {
    return x == y && y == z;
}

// Used as an example of a macro that evaluates to a boolean expression.
#define NUMBERS_EQ_5( Y, Z ) numbers_eq( 5, Y, Z )

// `TEST_REQUIRE` passes, and no further assertions are made.
static TestAssertion * ex_require_pass_1( void ) {
    for ( int i = 0; i < 10; i += 1 ) {
        TEST_REQUIRE( i != 11, i );
    }
    return NULL;
}

// `TEST_REQUIRE` passes, and an assertion is made afterwards.
static TestAssertion * ex_require_pass_2( void ) {
    for ( int i = 0; i < 10; i += 1 ) {
        TEST_REQUIRE( i != 11, i );
    }
    return test_assert( 1 == 1 );
}

// `TEST_REQUIRE` fails with one identification expressions.
static TestAssertion * ex_require_fail_1( void ) {
    for ( int i = 0; i < 10; i += 1 ) {
        TEST_REQUIRE( i != 7, i );
    }
    // Should not get here.
    return NULL;
}

// `TEST_REQUIRE` fails with three identification expressions.
static TestAssertion * ex_require_fail_2( void ) {
    int xs[] = { 1, 2, 4, 3, 8 };
    for ( size_t i = 0; i < ( sizeof xs ) - 1; i += 1 ) {
        TEST_REQUIRE( xs[ i ] < xs[ i + 1 ], i, xs[ i ], xs[ i + 1 ] );
    }
    // Should not get here.
    return test_assert( 1 + 1 == 2 );
}

// None of these assertions should be equal.
static TestAssertion const ex_as[] = {

    // `test_assertion_is_end__is_correct()` requires this to be first,
    // so you need to iterate over this array with `NELEM`.
    TEST_ASSERTIONS_END,

    // No identifications.
    { .expr = "",           .result = false, .num_ids = 0, .ids = NULL },
    { .expr = "1 + 1 == 2", .result = false, .num_ids = 0, .ids = NULL },
    { .expr = "blah",       .result = false, .num_ids = 0, .ids = NULL },
    { .expr = "",           .result = true,  .num_ids = 0, .ids = NULL },
    { .expr = "1 + 1 == 2", .result = true,  .num_ids = 0, .ids = NULL },
    { .expr = "blah",       .result = true,  .num_ids = 0, .ids = NULL },

    // One identification expression.
    { .expr = "",           .result = false, .num_ids = 1,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } },
    { .expr = "1 + 1 == 2", .result = false, .num_ids = 1,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } },
    { .expr = "blah",       .result = false, .num_ids = 1,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } },
    { .expr = "",           .result = true,  .num_ids = 1,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } },
    { .expr = "1 + 1 == 2", .result = true,  .num_ids = 1,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } },
    { .expr = "blah",       .result = true,  .num_ids = 1,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } },

    // Two identification expressions.
    { .expr = "",           .result = false, .num_ids = 2,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 },
                                      { .expr = "xs[ i ]", .value = -2 } } },
    { .expr = "1 + 1 == 2", .result = false, .num_ids = 2,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 },
                                      { .expr = "xs[ i ]", .value = -2 } } },
    { .expr = "blah",       .result = false, .num_ids = 2,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 },
                                      { .expr = "xs[ i ]", .value = -2 } } },
    { .expr = "",           .result = true,  .num_ids = 2,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 },
                                      { .expr = "xs[ i ]", .value = -2 } } },
    { .expr = "1 + 1 == 2", .result = true,  .num_ids = 2,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 },
                                      { .expr = "xs[ i ]", .value = -2 } } },
    { .expr = "blah",       .result = true,  .num_ids = 2,
        .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 },
                                      { .expr = "xs[ i ]", .value = -2 } } }

};

// ----------
// End of example functions and data.
// ----------



static
void * before_each( void )
{
    // Provide an array of assertion array pointers to each test, so
    // they can store references to their allocated assertion arrays
    // to be freed by `after_each`.
    return calloc( FREE_AFTER_SIZE, sizeof( TestAssertion * ) );
}


static
void after_each( void * const data )
{
    TestAssertion * * const to_free = data;
    for ( size_t i = 0; i < FREE_AFTER_SIZE; i += 1 ) {
        free( to_free[ i ] );
    }
    free( to_free );
}


static
TestAssertion * test_assertion_eq__is_correct_for_neq( void * const data )
{
    for ( size_t i = 0; i < NELEM( ex_as ); i += 1 ) {
        for ( size_t j = 0; j < NELEM( ex_as ); j += 1 ) {
            bool const eq = test_assertion_eq( ex_as[ i ], ex_as[ j ] )
                         && test_assertion_eq( ex_as[ j ], ex_as[ i ] );
            if ( j == i ) {
                TEST_REQUIRE( eq, i, j );
            } else {
                TEST_REQUIRE( !eq, i, j );
            }
        }
    }
    return NULL;
}


static
TestAssertion * test_assertion_eq__is_correct_for_eq( void * const data )
{
    // These assertions should be equal, because the identification
    // expressions are only compared for `num_ids` elements.
    TestAssertion const as[] = {
        { .expr = "1 + 1 == 2", .result = true,  .num_ids = 0, .ids = NULL },
        { .expr = "1 + 1 == 2", .result = true,  .num_ids = 0,
          .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } },
        { .expr = "1 + 1 == 2", .result = true,  .num_ids = 0,
          .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 },
                                        { .expr = "xs[ i ]", .value = -2 } }
        }
    };
    for ( size_t i = 0; i < NELEM( as ); i += 1 ) {
        for ( size_t j = 0; j < NELEM( as ); j += 1 ) {
            TEST_REQUIRE( test_assertion_eq( as[ i ], as[ j ] )
                       && test_assertion_eq( as[ j ], as[ i ] ), i, j );
        }
    }
    return NULL;
}


static
TestAssertion * test_assertion_is_end__is_correct( void * const data )
{
    for ( size_t i = 1; i < NELEM( ex_as ); i += 1 ) {
        TEST_REQUIRE( !test_assertion_is_end( ex_as[ i ] ), i );
    }
    return test_assert( test_assertion_is_end( ex_as[ 0 ] ) );
}


static
TestAssertion * test_assert__gives_right_assertions( void * const data )
{
    TestAssertion * * const to_free = data;
    TestAssertion * const as = to_free[ 0 ] = test_assert(
        4 / 2 == 3,
        numbers_eq( 12, 3 + 9, 24 / 2 ),
        NUMBERS_EQ_5( 2 + 3, 7 - 3 ) == false
    );
    return test_assert(
        test_assertion_eq( as[ 0 ], ( TestAssertion ){
            .expr = "4 / 2 == 3", .result = false
        } ),
        test_assertion_eq( as[ 1 ], ( TestAssertion ){
            .expr = "numbers_eq( 12, 3 + 9, 24 / 2 )", .result = true
        } ),
        // Macros are expanded before being stringified.
        test_assertion_eq( as[ 2 ], ( TestAssertion ){
            .expr = "numbers_eq( 5, 2 + 3, 7 - 3 ) == 0", .result = true
        } ),
        test_assertion_eq( as[ 3 ], ( TestAssertion ){ .expr = NULL } )
    );
}


static
TestAssertion * TEST_REQUIRE__returns_only_when_false( void * const data )
{
    TestAssertion * * const to_free = data;
    TestAssertion * const pass1 = ex_require_pass_1(); // == NULL
    TestAssertion * const pass2 = to_free[ 0 ] = ex_require_pass_2();
    TestAssertion * const fail1 = to_free[ 1 ] = ex_require_fail_1();
    TestAssertion * const fail2 = to_free[ 2 ] = ex_require_fail_2();
    return test_assert(

        // `ex_require_pass_1()` makes no assertions after its
        // `TEST_REQUIRE` loop.
        pass1 == NULL,

        // `ex_require_pass_2()` makes a single assertion with a single
        // expression after its `TEST_REQUIRE` loop.
        test_assertion_eq( pass2[ 0 ], ( TestAssertion ){
            .expr = "1 == 1", .result = true
        } ),
        test_assertion_is_end( pass2[ 1 ] ),

        // `ex_require_fails_1()` fails at a call to `TEST_REQUIRE` with
        // a single identification expression.
        test_assertion_eq( fail1[ 0 ], ( TestAssertion ){
            .expr = "i != 7", .result = false, .num_ids = 1,
            .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 7 } }
        } ),
        test_assertion_is_end( fail1[ 1 ] ),

        // `ex_require_fails_2()` fails at a call to `TEST_REQUIRE` with
        // three identification expressions.
        test_assertion_eq( fail2[ 0 ], ( TestAssertion ){
            .expr = "xs[ i ] < xs[ i + 1 ]", .result = false, .num_ids = 3,
            .ids = ( TestAssertionId[] ){
                { .expr = "i", .value = 2 },
                { .expr = "xs[ i ]", .value = 4 },
                { .expr = "xs[ i + 1 ]", .value = 3 }
            }
        } ),
        test_assertion_is_end( fail2[ 1 ] )

    );
}


Test const assertion_tests[] = TESTS_FIX( before_each, after_each,
    test_assertion_eq__is_correct_for_neq,
    test_assertion_eq__is_correct_for_eq,
    test_assertion_is_end__is_correct,
    test_assert__gives_right_assertions,
    TEST_REQUIRE__returns_only_when_false
);

