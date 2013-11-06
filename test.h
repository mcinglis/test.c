// test.h

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


#ifndef INCLUDED_TEST_H
#define INCLUDED_TEST_H


#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <macromap.h/macromap.h>        // MACROMAP, MACROMAP3


// An assertion is a boolean expression and its evaluation. An assertion
// may be identified so that it's easier to tell when and where it was
// made. This is useful when multiple assertions are made with the same
// expression, but different values (e.g. in a loop).
typedef struct TestAssertion {
    char const * expr;
    bool result;
    char const * id_expr;
    int id;
} TestAssertion;


// A test is a named function that generates an array of assertions. A
// test is considered to pass if all of these assertions were true.
typedef struct Test {

    // Used for displaying and naming the results of the test.
    char const * name;

    // Returns a dynamically-allocated terminated assertion array, or
    // null if the test makes no assertions. Returning null is commonly
    // used with `TEST_REQUIRE`, and tests that fail by prompting a
    // run-time error like a segmentation fault.
    TestAssertion * ( *func )( void * data );

    // If non-null, this is executed to generate the `data` argument to
    // pass to the test's `func` field.
    void * ( *before )( void );

    // If non-null, this is executed after calling the test's `func`,
    // and is passed the `data` generated by `before` and possibly
    // modified by `func`.
    void ( *after )( void * data );

} Test;


// Represents the results of running a set of tests.
typedef struct TestResults {
    int passed;
    int failed;
} TestResults;


// Returns true if the two assertions are equal: if they have the same
// expression, result, and identification. This only considers the
// logical values: not the memory addresses.
bool test_assert_eq( TestAssertion, TestAssertion );


// The end of an assertion array is an assertion with a null `expr`.
// These comments refer to an assertion array containing such an element
// as a "terminated assertion array".
#define TEST_ASSERTIONS_END { .expr = NULL }


// Copies a terminated assertion array onto the heap.
TestAssertion * test_assertions_alloc( TestAssertion const * assertions );


// Evaluates to a `TestAssertion` literal with the given expression.
#define TEST_ASSERTION( EXPR ) { .expr = #EXPR, .result = EXPR }


// Returns a dynamically-allocated terminated array of assertions
// corresponding to each of the given expressions. Because this uses
// `MACROMAP`, it can't take more than 128 expressions, and no
// expression can begin with more than four parentheses. See
// Macromap.h's documentation for more details.
#define test_assert( ... ) \
    test_assertions_alloc( ( TestAssertion[] ){ \
        MACROMAP( TEST_ASSERTIONS_EL, __VA_ARGS__ ) \
        TEST_ASSERTIONS_END \
    } )

// Prefixes a comma to separate each element of the array.
#define TEST_ASSERTIONS_EL( EXPR ) TEST_ASSERTION( EXPR ),


// This macro provides an easy way to assert things in a loop. If the
// given boolean expression evaluates to false, this causes the
// containing function to return that expression as a assertion (which
// will be false) as an element of a dynamically-allocated terminated
// array. The given expression will be identified by `ID` if `ID` is
// not a literal "0" (note that if it *evaluates* to 0, it will still be
// identified).
#define TEST_REQUIRE( EXPR, ID ) { \
    bool result = ( EXPR ); \
    if ( !result ) { \
        TestAssertion ta = TEST_ASSERTION( EXPR ); \
        if ( strcmp( #ID, "0" ) != 0 ) { \
            ta.id = ID; \
            ta.id_expr = #ID; \
        } \
        return test_assertions_alloc( ( TestAssertion[] ){ \
            ta, TEST_ASSERTIONS_END \
        } ); \
    } \
}


// Evaluates to a `Test` literal with the given test function
// expression, and named with the stringification of that expression.
#define TEST( FUNC ) { .func = FUNC, .name = #FUNC }

// Evaluates to a `Test` literal with the given arguments as fields,
// and `name` as a stringfication of the `FUNC` argument.
#define TEST_FIX( BEFORE, AFTER, FUNC ) \
    { .func = FUNC, .name = #FUNC, .before = BEFORE, .after = AFTER }


// The end of a test array is a test with a null `func`. These comments
// refer to an test array containing such an element as a "terminated
// test array".
#define TESTS_END { .func = NULL }


// Evaluates to a literal terminated array of tests corresponding to
// each of the given function expressions. Because this uses
// `MACROMAP`, it can't take more than 128 expressions, and no
// expression can begin with more than four parentheses. See
// Macromap.h's documentation for more details.
#define TESTS( ... ) { MACROMAP( TESTS_EL, __VA_ARGS__ ) TESTS_END }

// Prefixes a comma to separate each element of the array.
#define TESTS_EL( FUNC ) TEST( FUNC ),


// Similar to `TESTS`, but also takes two function expressions to set as
// the `before` and `after` fields of each of the tests. Because this
// uses `MACROMAP3`, it can't take more than 128 test function
// expressions, and no test function expression can begin with more than
// four parentheses. See Macromap.h's documentation for more details.
#define TESTS_FIX( BEFORE, AFTER, ... ) \
    { MACROMAP3( TESTS_FIX_EL, BEFORE, AFTER, __VA_ARGS__ ) \
      TESTS_END }

// Prefixes a comma to separate each element of the array.
#define TESTS_FIX_EL( BEFORE, AFTER, FUNC ) \
    TEST_FIX( BEFORE, AFTER, FUNC ),


// Runs each test in the terminated `tests` array, and prints the
// results to stdout, naming the set with `name`.
TestResults tests_run( char const * name, Test const * tests );


#endif // ifndef INCLUDED_TEST_H

