// test.h

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

#ifndef INCLUDED_TEST_H
#define INCLUDED_TEST_H


#include <stdlib.h>
#include <stdbool.h>

#include <macromap.h/macromap.h>        // MACROMAP


// An assertion is a boolean expression and its evaluation. An assertion
// may be identified so as to provide information beyond the expression.
// This is useful when multiple assertions are made with the same
// expression, but different values. Unique identifiers can make it
// easier to identify when and where the assertion failed.
typedef struct TestAssertion {
    char const * expr;
    bool result;
    bool has_id;
    int id;
} TestAssertion;


// A test is a named function that returns an array of assertions
// with an assertion with a null `expr` field as the end-sentinel.
typedef struct Test {
    char const * name;
    TestAssertion * ( *func )( void );
} Test;


// Returned by `tests_run` to communicate how many tests of its given
// array passed (i.e. all assertions were true) and how many failed.
typedef struct TestResults {
    int passed;
    int failed;
} TestResults;


// The end of an assertion array is an assertion with a null `expr`.
#define TEST_ASSERTIONS_END { .expr = NULL }


// Copies an array of assertions onto the heap.
TestAssertion * test_assertions_alloc( TestAssertion const * assertions );


// TODO: functions to append to and concatenate assertion arrays.


// Evaluates to an element of a `TestAssertion[]` literal, by
// stringifying the given expression. The trailing comma separates each
// element in the literal.
#define TEST_ASSERTIONS_EL( EXPR ) \
    ( TestAssertion ){ .expr = #EXPR, .result = ( EXPR ), .has_id = false },

// Takes a variable number of boolean expressions, and evaluates to a
// pointer to an array of assertions on the heap, with each assertion
// corresponding to a given expression. Note that because this uses
// `MACROMAP`, this can't take more than 128 arguments, and no argument
// can begin with more than four parentheses.
#define test_assert( ... ) \
    test_assertions_alloc( ( TestAssertion[] ){ \
        MACROMAP( TEST_ASSERTIONS_EL, __VA_ARGS__ ) \
        TEST_ASSERTIONS_END \
    } )


// TODO: interface to identify assertions similar to `test_assert`.


// Evaluates to code that checks if the given boolean expression
// evaluates to false, and if so, causes the containing function to
// return a pointer to an assertion array on the heap. This array
// will contain one assertion corresponding to the given expression and
// identified with the given `ID` argument.
#define TEST_REQUIRE( EXPR, ID ) { \
    bool result = ( EXPR ); \
    if ( !result ) { \
        return test_assertions_alloc( ( TestAssertion[] ){ \
            { .expr = #EXPR, .result = result, .has_id = true, .id = ID }, \
            TEST_ASSERTIONS_END \
        } ); \
    } \
}


// Evaluates to a `Test` literal with the given test function
// expression, and named with the stringification of that expression.
#define TEST( FUNC ) { .name = #FUNC, .func = FUNC }

// The end of a test array is a test with a null `func`.
#define TESTS_END { .func = NULL }

// Evaluates to an element of a `Test[]` literal. The trailing comma
// separates each element in the literal.
#define TESTS_EL( FUNC ) TEST( FUNC ),

// Takes a variable number of test function expressions, and evaluates
// to a `Test[]` literal with each test named as a stringification of
// its corresponding function expression. Note that because this uses
// `MACROMAP`, this can't take more than 128 arguments, and no argument
// can begin with more than 4 nested parentheses.
#define TESTS( ... ) { MACROMAP( TESTS_EL, __VA_ARGS__ ) TESTS_END }


// Runs each test in the `tests` array, and prints the results to
// stdout, with `name` printed as the name of the set of tests.
TestResults tests_run( char const * name, Test const * tests );


#endif // INCLUDED_TEST_H

