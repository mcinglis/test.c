// assertion.h

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


#ifndef INCLUDED_TESTC_ASSERTION_H
#define INCLUDED_TESTC_ASSERTION_H


#include <stdbool.h>
#include <stdlib.h>

#include <macromap.h/macromap.h> // MACROMAP


// An identification is an integer expression and its evaluation.
typedef struct TestAssertionId {

    char const * expr;
    int value;

    // Invariants:
    // - `expr` is not null

} TestAssertionId;


// An assertion is a boolean expression and its evaluation. An assertion
// may be identified so that it's easier to tell when and where it was
// made; this is used by `TEST_REQUIRE`.
typedef struct TestAssertion {

    // The boolean expression.
    char const * expr;

    // The evaluation of the boolean expression.
    bool result;

    // The array of identification expressions (or null).
    TestAssertionId * ids;

    // The size of the `ids` array.
    size_t num_ids;

    // Invariants:
    // - if `ids` is `NULL`, `num_ids` is `0`
    // - `ids` has at least as many elements as `num_ids`

} TestAssertion;


// Evaluates to a literal `TestAssertionId` with the given integer
// expression.
#define TEST_ASSERTION_ID( EXPR ) { .expr = #EXPR, .value = EXPR }


// Evaluates to a literal `TestAssertion` with the given boolean
// expression.
#define TEST_ASSERTION( EXPR ) { .expr = #EXPR, .result = EXPR }


// The sentinel value for the end of an assertion array. These comments
// refer to assertion arrays containing this value as being
// "terminated", because it signals to the receiver when to stop
// iterating through the array.
#define TEST_ASSERTIONS_END { .expr = NULL }


// Takes a series of boolean expressions, and evaluates to a literal,
// terminated `TestAssertion[]`. Because this depends on `MACROMAP`, it
// can't take more than 128 expressions, and no expression can begin
// with more than four parentheses. See Macromap.h's documentation for
// workarounds and more details.
#define TEST_ASSERTIONS( ... ) \
    { MACROMAP( TEST_ASSERTIONS_EL, __VA_ARGS__ ) TEST_ASSERTIONS_END }
#define TEST_ASSERTIONS_EL( EXPR ) TEST_ASSERTION( EXPR ),


// Returns true if the two assertions are equal, and false if not.
bool test_assertion_eq( TestAssertion, TestAssertion );


// Returns true if the given assertion is equal to
// `TEST_ASSERTIONS_END`.
bool test_assertion_is_end( TestAssertion );


// Copies the given terminated assertion array into allocated memory,
// and returns a pointer to that memory. Each of the assertions with
// non-zero `num_ids` fields will have their `ids` arrays copied into
// allocated memory for as many elements as `num_ids`.
TestAssertion * test_assertions_alloc( TestAssertion const * assertions );


// Frees the memory allocated for each assertion's identifications
// array, and the memory allocated for the assertion itself.
void test_assertions_free( TestAssertion * const assertions );


// Returns an allocated, terminated assertion array corresponding to the
// given series of boolean expressions.
//
// Because this depends on `MACROMAP`, it can't take more than 128
// expressions, and no expression can begin with more than four
// parentheses. See Macromap.h's documentation for workarounds and more
// details.
#define test_assert( ... ) \
    test_assertions_alloc( \
        ( TestAssertion[] ) TEST_ASSERTIONS( __VA_ARGS__ ) \
    )


// This macro provides an easy way to assert things in a loop. If the
// given boolean expression evaluates to false, this macro causes the
// containing function to return that expression as an assertion (which
// will be false) as the single element of an allocated, terminated
// assertion array. The assertion will be identified by the given series
// of integral expressions if the first such expression is not a literal
// `0`.
//
// Because this depends on `MACROMAP`, it can't take more than 128
// identification expressions, and no expression can begin with more
// than four parentheses. See Macromap.h's documentation for workarounds
// and more details.
#define TEST_REQUIRE( EXPR, ... ) \
    { \
        bool result = ( EXPR ); \
        if ( !result ) { \
            TestAssertionId ids[] = { \
                MACROMAP( TEST_ASSERTION_IDS_EL, __VA_ARGS__ ) { 0 } \
            }; \
            return test_assertions_alloc( ( TestAssertion[] ){ \
                { .expr = #EXPR, \
                  .result = result, \
                  .ids = ids, \
                  .num_ids = ( sizeof( ids ) / sizeof( ids[ 0 ] ) ) - 1 }, \
                TEST_ASSERTIONS_END \
            } ); \
        } \
    }
#define TEST_ASSERTION_IDS_EL( EXPR ) TEST_ASSERTION_ID( EXPR ),

// There is trailing zeroed literal in the initialization of `ids`
// because `MACROMAP` will evaluate to an expression with a trailing
// comma. We effectively remove this element by subtracting one from the
// size of `ids` when assigning to `num_ids`, because
// `test_assertions_alloc` will only allocate memory for that many
// elements.
//
// Also, `test_assertions_alloc` will handle the case when the first
// identification expression is a literal `0`.


#endif // ifndef INCLUDED_TESTC_ASSERTION_H

