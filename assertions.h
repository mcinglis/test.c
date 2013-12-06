// assertions.h

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


#ifndef INCLUDED_TESTC_ASSERTIONS_H
#define INCLUDED_TESTC_ASSERTIONS_H


#include <stdbool.h>
#include <stdlib.h>

#include "assertion-id.h" // AssertionId, ASSERTION_ID_ARRAY
#include "assertion.h" // Assertion, ASSERTION_ARRAY, assertion_new


// An array-backed sequence of `Assertion` pointers.
typedef struct Assertions {

    // A pointer to an array of assertion pointers.
    Assertion * * array;

    // How many sequential elements from the start of the array are
    // considered valid, i.e., how many `Assertion *`s are currently
    // contained.
    size_t size;

    // The total capacity of the `array`, i.e., how many `Assertion *`s
    // it can hold before we need to reallocate it.
    size_t capacity;

    // Invariants:
    // - `size` is always less than or equal to `capacity`
    // - `array` is `NULL` if and only if `capacity` is `0`
    // - `array[ i ]` is not `NULL` for all `0 <= i < size`

} Assertions;


// Returns `true` if the invariants hold for the given `Assertions`, or
// `false` if some don't.
bool assertions_is_valid( Assertions );


// Asserts that the invariants hold for the given `Assertions`.
void assertions_assert_valid( Assertions );


// The initial `capacity` for an allocated `Assertions`. Returned
// `Assertions` may have a larger capacity, but this is what they will
// be initially constructed with.
extern size_t const assertions_initial_capacity;


struct assertions_new_options {
    Assertion const * array;
    size_t capacity;
};

Assertions * assertions_new_( struct assertions_new_options );

// Allocates and returns a new `Assertions` with a copy of each of the
// `Assertion`s in the given `array` up to (but not including) the
// terminating element with a `NULL` `expr` field, with the
// corresponding `size`. The `Assertions` will be iniitally allocated
// with the given `capacity` (or `assertions_initial_capacity` if the
// given `capacity` is `0`, i.e., not given), but this will be increased
// if needed. If the given `array` is `NULL`, then the returned
// `Assertions` will be empty.
#define assertions_new( ... ) \
    assertions_new_( ( struct assertions_new_options ){ \
        __VA_ARGS__ \
    } )


// Allocates and returns an `Assertions` containing no assertions, with
// a `capacity` of `assertions_initial_capacity`.
Assertions * assertions_empty( void );


// Allocates and returns a new `Assertions` with a copy of each of the
// `Assertion`s in the given array (but not the terminating element with
// a `NULL` `expr` field), the corresponding `size`, and an appropriate
// `capacity`.
Assertions * assertions_( Assertion const * array );


// Takes a variable number of `bool` expressions, and allocates and
// returns a new `Assertions` with an `array` holding `Assertion`s
// corresponding to the given expressions, the corresponding `size`, and
// an appropriate `capacity`.
//
// This uses `ASSERTION_ARRAY`, which depends on `MACROMAP`, so it can't
// take more than 128 expressions, and no expression can begin with more
// than four parentheses.
#define assertions( ... ) \
    assertions_new( \
        .array = ( Assertion[] ) ASSERTION_ARRAY( __VA_ARGS__ ) \
    )


// Copies the given `Assertions` value into allocated memory, and
// returns a pointer to that memory. This deeply copies the internal
// `Assertion`s, so that any changes to the original or its pointees
// will not change the copy or its pointees. The returned `Assertions`
// will have the same `capacity` as the given one.
Assertions * assertions_copy( Assertions const as );


// Frees the memory allocated for each of the `Assertion`s in the given
// `Assertions`'s `array`, and the memory allocated for the `Assertions`
// itself.
void assertions_free( Assertions * const assertions );


// Returns `true` if the two `Assertions` have the same `size`, with the
// same `Assertion`s in the same order in their `array`s. Otherwise,
// returns `false`.
bool assertions_eq( Assertions const as1, Assertions const as2 );


// Returns `true` if the given `Assertion` has equal `Assertion`s
// (according to `assertion_eq()`) in the same order as the given
// `array`, up to the terminating element with a `NULL` `expr` field,
// and that the given `Assertions` has no further elements. Otherwise,
// returns `false`.
bool assertions_eq_array( Assertions, Assertion const * array );


// Takes an `Assertions` and a variable number of `bool` expressions,
// and returns `true` if the given `Assertions` has equal `Assertion`s
// (according to `assertion_eq()`) in the order given.
#define assertions_eq_each( ASSERTIONS, ... ) \
    assertions_eq_array( ASSERTIONS, \
        ( Assertion[] ) ASSERTION_ARRAY( __VA_ARGS__ ) \
    )


struct assertions_print_options {
    Assertions assertions;
    FILE * file;
    char const * assertion_indent;
    char const * ids_indent;
};

void assertions_print_( bool result, struct assertions_print_options );

// Prints the assertions of the given `assertions` to the `file` (or
// `stdout` if `NULL`) with the given `RESULT`, indenting each assertion
// line with `assertion_indent` (or `""` if `NULL`), and each
// identification line with `ids_indent` (or `""` if `NULL`).
#define assertions_print( RESULT, ... ) \
    assertions_print_( RESULT, ( struct assertions_print_options ){ \
        __VA_ARGS__ \
    } )


// Returns `true` if all of the `Assertion`s in the `array` of the given
// value have a `true` `result` field. Otherwise, returns `false`.
bool assertions_all_true( Assertions );


// Increases the `capacity` of the given `Assertions`, and reallocates
// the `array` accordingly.
void assertions_increase_capacity( Assertions * const as );


// Halves the `capacity` of the given `Assertions`, frees the elements
// that are being lost (if any), and reallocates the `array`
// accordingly.
void assertions_decrease_capacity( Assertions * const as );


// If `index >= 0`, it should be less than the `size` of the given
// `Assertions`, and this will return `array[ index ]`. If `index < 0`,
// it should be greater or equal to `-size`, and this will return
// `array[ size + index ]`.
Assertion * assertions_get( Assertions, long long index );


// Adds a copy of the given `Assertion` to the given `Assertions`,
// increasing the capacity if necessary, and increments the `size`.
void assertions_add_( Assertions * assertions, Assertion );


// Takes an `Assertions *`, a `bool` expression, and a variable number
// of `int` expressions for identification, and adds an `Assertion` with
// that given `bool` expression, identified with  those given `int`
// expressions, to the given `Assertions`. This is commonly used in
// loops to create assertions for a range of values. If the first
// identification expression is a literal `0`, then the added
// `Assertion` will be given no identification expressions - this
// prevents printing an identification line.
//
// This depends on `MACROMAP`, so it can't take more than 128
// identification expressions, and no expression can begin with more
// than four parentheses.
#define assertions_add( ASSERTIONS, EXPR, ... ) \
    assertions_add_ptr( ASSERTIONS, assertion_new( EXPR, __VA_ARGS__ ) )


// Adds the given `Assertion *` to the given `Assertions` (without
// copying), increasing the capacity if necessary, and increments the
// `size`. To satisfy the invariants, the given `Assertion *` can't be
// `NULL`.
void assertions_add_ptr( Assertions * assertions, Assertion * assertion );


// Adds all of the `Assertion`s in the given `array` to the given
// `Assertions` up to (but not including) the terminating element with a
// `NULL` `expr` field. This increases the `size` and `capacity` of the
// given `Assertions` as appropriate.
void assertions_add_all( Assertions * assertions, Assertion const * array );


#endif // ifndef INCLUDED_TESTC_ASSERTIONS_H

