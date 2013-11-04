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


// An assertion is a boolean expression and its evaluation. It may be
// identified by an integer, so as to provide more information than its
// expression (e.g. the expression may consist of variables, but readers
// can't know the values of those variables without a way to number each
// assertion).
typedef struct TestAssertion {
    char const * expr;
    bool result;
    bool has_id;
    int id;
} TestAssertion;


// A test is a named function that returns an array of assertions, which
// may be true or false.
typedef struct Test {
    // Returns an array of assertions terminated by an assertion with
    // a null `expr` field.
    TestAssertion * ( *func )( void );
    // Used for printing the results of running this test.
    char const * name;
} Test;


// Counts of the tests that passed and the tests that failed. A value of
// this struct is returned by `test_run_all`.
typedef struct TestResults {
    int passed;
    int failed;
} TestResults;


// Copies an array of assertions, terminated by an assertion with a null
// expression field, onto the heap.
TestAssertion * test_assert_alloc( TestAssertion const * assertions );

// Creates an element of an assertion list by stringifying the given
// expression. The trailing comma separates each element when building
// an array of assertions with `MACROMAP`.
#define TEST_ASSERT_EL( EXPR ) \
    ( TestAssertion ){ .expr = #EXPR, .result = ( EXPR ), .has_id = false },

// Takes a variable number of expressions, and returns a pointer to an
// array of assertions on the heap, with each assertion corresponding to
// a given expression.
#define test_assert( ... ) \
    test_assert_alloc( ( TestAssertion[] ){ \
        MACROMAP( TEST_ASSERT_EL, __VA_ARGS__ ) \
        { .expr = NULL } \
    } )

// If the given expression evaluates to false, then this causes the
// containing function to return am assertion array with an assertion
// corresponding to the given expression. This is an easy way to assert
// things in a loop. The returned array will be terminated with an
// expression with a null `expr` field. The single assertion will be
// identified with the given `id` argument.
#define TEST_REQUIRE( EXPR, ID ) { \
    bool result = ( EXPR ); \
    if ( !result ) { \
        return test_assert_alloc( ( TestAssertion[] ){ \
            { .expr = #EXPR, .result = result, .has_id = true, .id = ID }, \
            { .expr = NULL } \
        } ); \
    } \
}


#define TEST( FUNC ) { .name = #FUNC, .func = FUNC }
#define TEST_END { .name = NULL, .func = NULL }
#define TEST_EL( FUNC ) TEST( FUNC ),
#define TESTS( ... ) { MACROMAP( TEST_EL, __VA_ARGS__ ) TEST_END }


TestResults test_run_all( char const * name, Test const * tests );


#endif // INCLUDED_TEST_H
