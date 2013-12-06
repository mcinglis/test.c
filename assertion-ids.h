// assertion-ids.h

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


#ifndef INCLUDED_TESTC_ASSERTION_IDS_H
#define INCLUDED_TESTC_ASSERTION_IDS_H


#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "assertion-id.h" // AssertionId


// An array-backed sequence of `AssertionId`s.
typedef struct AssertionIds {

    // A pointer to an array of identifications.
    AssertionId * array;

    // How many sequential elements from the start of the array are
    // considered valid, i.e., how many `AssertionId`s are currently
    // contained.
    size_t size;

    // The total capacity of the `array`, i.e., how many `AssertionId`s
    // it can hold before we need to reallocate it.
    size_t capacity;

    // Invariants:
    // - `size` is always less than or equal to `capacity`
    // - `array` is `NULL` if and only if `capacity` is `0`
    // - `array[ i ]` is not `NULL` for all `0 <= i < size`

} AssertionIds;


// The initial `capacity` for an allocated `AssertionIds`. Returned
// `AssertionIds` may have a larger capacity, but this is what they will
// be initially constructed with.
extern size_t const assertion_ids_initial_capacity;


// Returns `true` if the invariants hold for the given `AssertionIds`, or
// `false` if some don't.
bool assertion_ids_is_valid( AssertionIds const ids );


// Asserts that the invariants hold for the given `AssertionIds`.
void assertion_ids_assert_valid( AssertionIds const ids );


struct assertion_ids_new_options {
    AssertionId const * array;
    size_t capacity;
};

AssertionIds * assertion_ids_new_( struct assertion_ids_new_options );

// Allocates and returns a new `AssertionIds` with at least the given
// `capacity`, and containing the values from the given `array`, which
// should be terminated in the same fashion as `ASSERTION_ID_ARRAY()`.
// If the given `array` is `NULL`, then the returned `AssertionIds` will
// contain no elements. If the given `capacity` is less than the length
// of the `array`, then the returned `AssertionIds` will have a
// `capacity` that's large enough to store the entire given `array`.
// Otherwise, if the given `capacity` is greater than or equal to the
// length of the `array`, then the returned `AssertionIds` will have
// that `capacity`.
#define assertion_ids_new( ... ) \
    assertion_ids_new_( ( struct assertion_ids_new_options ){ \
        __VA_ARGS__ \
    } )


// Allocates and returns an `AssertionIds` containing no elements, with
// a `capacity` of `assertion_ids_initial_capacity`.
AssertionIds * assertion_ids_empty( void );


// Takes a variable number of `int` expressions, and allocates and
// returns a new `AssertionIds` containing `AssertionId`s corresponding
// to the given expressions, in the order given. You can pass a literal
// `0` as the first argument to receive an empty `AssertionIds`.
//
// This depends on `MACROMAP`, so it can't take more than 128
// expressions, and no expression can begin with more than four
// parentheses.
#define assertion_ids( ... ) \
    assertion_ids_new( \
        .array = ( AssertionId[] ) ASSERTION_ID_ARRAY( __VA_ARGS__ ) \
    )


// Copies the given `AssertionIds` into allocated memory, and returns a
// pointer to that memory. This deeply copies the non-`const` pointees
// of the given `AssertionIds`, so that any changes to the original or
// its pointees will not change the copy or its pointees.
AssertionIds * assertion_ids_copy( AssertionIds );


// Frees the memory allocated for the given `AssertionIds` and its
// `array`.
void assertion_ids_free( AssertionIds * ids );


// Returns `true` if the two `AssertionIds` are equal, and `false` if
// not.
bool assertion_ids_eq( AssertionIds, AssertionIds );


// Returns `true` if the given `AssertionIds` contains equal
// `AssertionId`s (according to `assertion_id_eq()`) in the same order
// as the given `array` (which should be terminated in the same fashion
// as `ASSERTION_ID_ARRAY()`), and has no further elements. Otherwise,
// returns `false`.
bool assertion_ids_eq_array( AssertionIds, AssertionId const * array );


// Takes an `AssertionIds` and a variable number of `int` expressions, and
// returns `true` if the given `AssertionIds` has equal `AssertionId`s
// (according to `assertion_id_eq()`) in the order given.
#define assertion_ids_eq_each( IDS, ... ) \
    assertion_ids_eq_array( IDS, \
        ( AssertionId[] ) ASSERTION_ID_ARRAY( __VA_ARGS__ ) \
    )


// Returns `true` if the given `AssertionIds` is empty (`size == 0`),
// and `false` otherwise.
bool assertion_ids_is_empty( AssertionIds );


// Increases the `capacity` of the given `AssertionIds`, and reallocates
// the `array` accordingly.
void assertion_ids_increase_capacity( AssertionIds * ids );


// Halves the `capacity` of the given `AssertionIds` (via integer
// division by 2), and reallocates the `array` accordingly.
void assertion_ids_decrease_capacity( AssertionIds * ids );


// If `index >= 0`, it should be less than the `size` of the given
// `AssertionIds`, and this will return `array[ index ]`. If
// `index < 0`, it should be greater or equal to `-size`, and this will
// return `array[ size + index ]`.
AssertionId assertion_ids_get( AssertionIds, long long index );


// Adds the given `AssertionId` to the given `AssertionIds`, increasing
// the `capacity` if necessary, and increments the `size`.
void assertion_ids_add( AssertionIds * ids, AssertionId );


// Adds all of the `AssertionId`s in the given `array`, terminated as
// per `ASSERTION_ID_ARRAY()`, to the given `AssertionIds`, increasing
// the `size` and `capacity` as appropriate.
void assertion_ids_add_all( AssertionIds * ids, AssertionId const * array );


struct assertion_ids_print_options {
    AssertionIds ids;
    FILE * file;
};

void assertion_ids_print_( struct assertion_ids_print_options );

// Prints the given `ids` to the given `file` (or `stdout` if `NULL`).
// For example:
//      assertion_ids_print( assertion_ids( 1, 2, 3 ) )
// Will print:
//      (for 1 = 1, 2 = 2, 3 = 3)
#define assertion_ids_print( ... ) \
    assertion_ids_print_( ( struct assertion_ids_print_options ){ \
        __VA_ARGS__ \
    } )


#endif // ifndef INCLUDED_TESTC_ASSERTION_IDS_H

