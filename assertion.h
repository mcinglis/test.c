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
#include <stdio.h>

#include <macromap.h/macromap.h> // MACROMAP

#include "assertion-ids.h" // AssertionIds


// An assertion is a boolean expression and its evaluation. An assertion
// may be identified so that it's easier to tell when and where it was
// made.
typedef struct Assertion {

    // The boolean expression.
    char const * expr;

    // The evaluation of the boolean expression.
    bool result;

    // The sequence of identifications.
    AssertionIds * ids;

    // Invariants:
    // - `expr` is not `NULL`

} Assertion;


// Evaluates to a literal `Assertion` with the given `bool` expression.
#define ASSERTION( EXPR ) { .expr = #EXPR, .result = EXPR }


// Takes a series of `bool` expressions, and evaluates to a literal
// `Assertion[]` terminated by an `Assertion` with a `NULL` `expr`
// field (which violates an invariant of `Assertion`, but is intended to
// only be used as an end-sentinel, and otherwise ignored). This
// provides an array as expected by `assertions_new()` and
// `assertions_add_all()`.
//
// This depends on `MACROMAP`, so it can't take more than 128
// expressions, and no expression can begin with more than four
// parentheses.
#define ASSERTION_ARRAY( ... ) \
    { MACROMAP( ASSERTION_ARRAY_EL, __VA_ARGS__ ) { .expr = NULL } }
#define ASSERTION_ARRAY_EL( EXPR ) ASSERTION( EXPR ),


// Returns `true` if the invariants hold for the given `Assertion`, or
// `false` if some don't.
bool assertion_is_valid( Assertion );


// Asserts that the invariants hold for the given `Assertion`.
void assertion_assert_valid( Assertion );


struct assertion_new_options {
    char const * expr;
    bool result;
    AssertionId const * ids;
};

// Allocates and returns a new assertion with the given fields, and
// identified with a copy of the given `ids` array which should be
// terminated in the same fashion as `ASSERTION_ID_ARRAY()`. If the
// given `ids` is `NULL`, then the returned assertion will have an empty
// (but non-null) `ids`.
Assertion * assertion_new_( struct assertion_new_options );

// Allocates and returns a new `Assertion` with the given `bool`
// expression, and identified with the variable number of given
// identifying `int` expressions.
#define assertion_new( EXPR, ... ) \
    assertion_new_( ( struct assertion_new_options ){ \
        .expr = #EXPR, \
        .result = EXPR, \
        .ids = ( AssertionId[] ) ASSERTION_ID_ARRAY( __VA_ARGS__ ) \
    } )


// Copies the given `Assertion` into allocated memory, and returns a
// pointer to that memory. This deeply copies the `ids` of the given
// `Assertion`, so that any changes to the original or its pointees
// won't change the copy or its pointees.
Assertion * assertion_copy( Assertion );


// Frees the memory allocated for the assertion's identifications
// array, and the memory allocated for the assertion itself.
void assertion_free( Assertion * const assertion );


// Returns `true` if the two assertions are equal, and `false` if not.
bool assertion_eq( Assertion, Assertion );


// Returns `true` if the given assertion has any identifications, and
// `false` if not.
bool assertion_has_ids( Assertion );


struct assertion_print_options {
    Assertion assertion;
    FILE * file;
    char const * ids_indent;
};

void assertion_print_( struct assertion_print_options );

// Prints the `assertion` to the `file` (or `stdout` if `NULL`). If the
// assertion has identifications, this will print those identifications
// on a subsequent line indented by `ids_indent` (or by `""` if `NULL`).
#define assertion_print( ... ) \
    assertion_print_( ( struct assertion_print_options ){ \
        __VA_ARGS__ \
    } )


#endif // ifndef INCLUDED_TESTC_ASSERTION_H

