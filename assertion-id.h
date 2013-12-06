// assertion-id.h

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


#ifndef INCLUDED_TESTC_ASSERTION_ID_H
#define INCLUDED_TESTC_ASSERTION_ID_H


#include <stdbool.h>

#include <macromap.h/macromap.h> // MACROMAP


// An assertion identification is an `int` expression and its result.
typedef struct AssertionId {

    // The stringification of the `int` expression.
    char const * expr;

    // The result of the `int` expression.
    int value;

    // Invariants:
    // - `expr` is not null

} AssertionId;


// Evaluates to a literal `AssertionId` with the given `int` expression.
#define ASSERTION_ID( EXPR ) { .expr = #EXPR, .value = EXPR }


// The terminating element of an `AssertionId[]` as given by
// `ASSERTION_ID_ARRAY()`.
#define ASSERTION_ID_ARRAY_END { .expr = "0" }

// Global constant to save typecasting `ASSERTION_ID_ARRAY_END`.
extern AssertionId const assertion_id_array_end;


// Takes a series of `int` expressions, and evaluates to a literal
// `AssertionId[]` with those expressions, terminated by
// `ASSERTION_ID_ARRAY_END`.
//
// This depends on `MACROMAP`, so it can't take more than 128
// expressions, and no expression can begin with more than four
// parentheses.
#define ASSERTION_ID_ARRAY( ... ) \
    { MACROMAP( ASSERTION_ID_ARRAY_EL, __VA_ARGS__ ) \
      ASSERTION_ID_ARRAY_END }
#define ASSERTION_ID_ARRAY_EL( EXPR ) ASSERTION_ID( EXPR ),


// Returns `true` if the invariants hold for the given `AssertionId`, or
// `false` if some don't.
bool assertion_id_is_valid( AssertionId );


// Asserts that the invariants hold for the given `AssertionId`.
void assertion_id_assert_valid( AssertionId );


// Returns `true` if the two identifications have the same `value` and
// equivalent `expr`s. Otherwise, returns `false`.
bool assertion_id_eq( AssertionId, AssertionId );


// Returns `true` if the given identification is equal to
// `assertion_id_array_end`.
bool assertion_id_is_array_end( AssertionId );


#endif // ifndef INCLUDED_TESTC_ASSERTION_ID_H

