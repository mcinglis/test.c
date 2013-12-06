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


#ifndef INCLUDED_TESTC_TEST_H
#define INCLUDED_TESTC_TEST_H


#include <stdbool.h>
#include <stdio.h>

#include <macromap.h/macromap.h> // MACROMAP, MACROMAP2

#include "assertions.h" // Assertions


typedef Assertions * ( * test_fn )( void );


// A test is a named function that generates an array of assertions. A
// test is considered to pass if all of these assertions were true.
typedef struct Test {

    // Used for displaying and naming the results of the test.
    char const * name;

    // Generates and returns a sequence of assertions.
    test_fn func;

    // Invariants:
    // - `name` is not `NULL`
    // - `func` is not `NULL`
    // - `func()` is not `NULL`

} Test;


// Evaluates to a literal `Test` with the given function expression.
#define TEST( FUNC ) { .func = FUNC, .name = #FUNC }


// Takes a series of `test_fn` expressions, and evaluates to a literal
// `Test[]` terminated by an `Test` with a `NULL` `func` field (which
// violates an invariant of `Assertion`, but is intended to only be used
// as an end-sentinel, and otherwise ignored).
//
// This depends on `MACROMAP`, so it can't take more than 128
// expressions, and no expression can begin with more than four
// parentheses.
#define TEST_ARRAY( ... ) \
    { MACROMAP( TEST_ARRAY_EL, __VA_ARGS__ ) { .func = NULL } }
#define TEST_ARRAY_EL( FUNC ) TEST( FUNC ),


// Returns `true` if the two given tests are equal, and `false` if not.
bool test_eq( Test, Test );


struct test_run_options {
    Test test;
    FILE * file;
    char const * indent;
};

// Runs the given `test`, prints the results to `file` (or `stdout` if
// `NULL`), indenting each line with `indent` (or `""` if `NULL`), and
// returns `true` if the test passed or `false` if it failed.
bool test_run_( struct test_run_options );
#define test_run( ... ) \
    test_run_( ( struct test_run_options ){ __VA_ARGS__ } )


struct tests_run_options {
    char const * name;
    Test const * tests;
    FILE * file;
    char const * indent;
};

// Runs each test in the terminated `tests` array, prints the results to
// `file` (or `stdout` if `NULL`), indenting each line with `indent` (or
// `"  "` if `NULL`), and returns the number of failures.
int tests_run_( struct tests_run_options );
#define tests_run( ... ) \
    tests_run_( ( struct tests_run_options ){ __VA_ARGS__ } )


// Returns `1` if any of the integers in the given array (up to the
// terminating `-1`) are non-zero (i.e. there was a failure), and `0` if
// they're all zero.
int tests_return_val_( int const * fail_counts );


// Returns `1` if any of the given `int` expressions were non-zero, or
// `0` if they were all zero.
#define tests_return_val( ... ) \
    tests_return_val_( ( int[] ){ __VA_ARGS__, -1 } )


#endif // ifndef INCLUDED_TESTC_TEST_H

