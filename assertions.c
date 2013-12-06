// assertion.c

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


#include "assertions.h" // Assertions

#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "_common.h" // string_eq
#include "assertion.h" // Assertion, assertion_*


size_t const assertions_initial_capacity = 32;


static
bool array_is_null_iff_capacity_is_zero( Assertions const as )
// Checks an invariant condition.
{
    return ( as.capacity == 0 && as.array == NULL )
        || ( as.capacity != 0 && as.array != NULL );
}


static
bool all_elements_up_to_size_are_not_null( Assertions const as )
// Checks an invariant condition.
{
    for ( size_t i = 0; i < as.size; i += 1 ) {
        if ( as.array[ i ] == NULL ) {
            return false;
        }
    }
    return true;
}


static
bool all_elements_are_valid( Assertions const as )
// Checks an invariant condition.
{
    for ( size_t i = 0; i < as.size; i += 1 ) {
        if ( !assertion_is_valid( *( as.array[ i ] ) ) ) {
            return false;
        }
    }
    return true;
}


bool assertions_is_valid( Assertions const as )
{
    return as.size <= as.capacity
        && array_is_null_iff_capacity_is_zero( as )
        && all_elements_up_to_size_are_not_null( as )
        && all_elements_are_valid( as );
}


void assertions_assert_valid( Assertions const as )
{
    assert( as.size <= as.capacity );
    assert( array_is_null_iff_capacity_is_zero( as ) );
    assert( all_elements_up_to_size_are_not_null( as ) );
    // To get better assertion errors:
    for ( size_t i = 0; i < as.size; i += 1 ) {
        assertion_assert_valid( *( as.array[ i ] ) );
    }
}


Assertions * assertions_new_( struct assertions_new_options const o )
{
    Assertion const * const array = o.array;
    size_t const capacity =
        ( o.capacity == 0 ) ? assertions_initial_capacity : o.capacity;

    Assertions * const as = malloc( sizeof ( Assertions ) );
    *as = ( Assertions ){
        .size = 0,
        .capacity = capacity,
        .array = malloc( capacity * sizeof ( Assertion * ) )
    };
    if ( array != NULL ) {
        assertions_add_all( as, array );
    }
    return as;
}


Assertions * assertions_empty( void )
{
    return assertions_new( .capacity = assertions_initial_capacity );
}


Assertions * assertions_copy( Assertions const as )
{
    assertions_assert_valid( as );
    Assertions * const copy = assertions_new( .capacity = as.capacity );
    for ( size_t i = 0; i < as.size; i += 1 ) {
        assertions_add_( copy, *assertions_get( as, i ) );
    }
    return copy;
}


void assertions_free( Assertions * const as )
{
    if ( as != NULL ) {
        assertions_assert_valid( *as );
        for ( size_t i = 0; i < as->size; i += 1 ) {
            assertion_free( as->array[ i ] );
        }
        free( as->array );
        free( as );
    }
}


bool assertions_eq( Assertions const as1, Assertions const as2 )
{
    assertions_assert_valid( as1 );
    assertions_assert_valid( as2 );

    if ( as1.size != as2.size ) {
        return false;
    }
    for ( size_t i = 0; i < as1.size; i += 1 ) {
        if ( !assertion_eq( *assertions_get( as1, i ),
                            *assertions_get( as2, i ) ) ) {
            return false;
        }
    }
    return true;
}


bool assertions_eq_array( Assertions const as, Assertion const * const array )
{
    assertions_assert_valid( as );
    Assertions * const new = assertions_new( .array = array );
    bool const eq = assertions_eq( as, *new );
    assertions_free( new );
    return eq;
}


void assertions_print_( bool const result,
                        struct assertions_print_options const o )
{
    Assertions const as = o.assertions;
    assertions_assert_valid( as );
    FILE * const file = ( o.file == NULL ) ? stdout : o.file;
    char const * const assertion_indent =
        ( o.assertion_indent == NULL ) ? "" : o.assertion_indent;
    char const * const ids_indent =
        ( o.ids_indent == NULL ) ? "" : o.ids_indent;

    char const * last_expr = NULL;
    for ( size_t i = 0; i < as.size; i += 1 ) {
        Assertion const a = *assertions_get( as, i );
        if ( a.result == result ) {
            // Don't repeat consecutive equal assertion expressions;
            // just print the identifications (if any).
            if ( last_expr != NULL && strcmp( a.expr, last_expr ) == 0 ) {
                // Separate `if` block so that we don't print the
                // assertion expression if it has no identifications.
                if ( assertion_has_ids( a ) ) {
                    fprintf( file, "%s", ids_indent );
                    assertion_ids_print( .ids = *( a.ids ), .file = file );
                }
            } else {
                fprintf( file, "%s", assertion_indent );
                assertion_print( .assertion = a,
                                 .file = file,
                                 .ids_indent = ids_indent );
            }
            last_expr = a.expr;
        }
    }
}


bool assertions_all_true( Assertions const as )
{
    assertions_assert_valid( as );
    for ( size_t i = 0; i < as.size; i += 1 ) {
        if ( assertions_get( as, i )->result == false ) {
            return false;
        }
    }
    return true;
}


void assertions_increase_capacity( Assertions * const as )
{
    assert( as != NULL );
    assertions_assert_valid( *as );

    as->capacity = ( as->capacity == 0 ) ? assertions_initial_capacity
                                         : as->capacity * 2;
    as->array = realloc( as->array, as->capacity * sizeof ( Assertion * ) );
}


void assertions_decrease_capacity( Assertions * const as )
{
    assert( as != NULL );
    assertions_assert_valid( *as );

    if ( as->capacity == 0 ) {
        return;
    }
    as->capacity /= 2;
    // If we're decreasing the capacity past the current size of the
    // array, free those elements we're losing.
    if ( as->capacity < as->size ) {
        for ( size_t i = as->capacity; i < as->size; i += 1 ) {
            assertion_free( as->array[ i ] );
        }
        as->size = as->capacity;
    }
    as->array = realloc( as->array, as->capacity * sizeof ( Assertion * ) );
    if ( as->capacity == 0 ) {
        // `realloc` may not return `NULL` if the given `size` is `0`,
        // but it will certainly free the given pointer.
        as->array = NULL;
    }
}


Assertion * assertions_get( Assertions const as, long long const i )
{
    assertions_assert_valid( as );

    long long const lsize = as.size;
    assert( -lsize <= i && i < lsize );
    return as.array[ ( i < 0 ) ? ( lsize + i ) : i ];
}


void assertions_add_( Assertions * const as, Assertion const a )
{
    assert( as != NULL );
    assertions_assert_valid( *as );
    assertion_assert_valid( a );

    if ( as->size == as->capacity ) {
        assertions_increase_capacity( as );
    }
    as->array[ as->size ] = assertion_copy( a );
    as->size += 1;
}


void assertions_add_ptr( Assertions * const as, Assertion * const a )
{
    assert( as != NULL );
    assertions_assert_valid( *as );
    assert( a != NULL );
    assertion_assert_valid( *a );

    if ( as->size == as->capacity ) {
        assertions_increase_capacity( as );
    }
    as->array[ as->size ] = a;
    as->size += 1;
}


void assertions_add_all( Assertions * const as, Assertion const * const array )
{
    assert( as != NULL );
    assertions_assert_valid( *as );
    assert( array != NULL );

    for ( size_t i = 0; array[ i ].expr != NULL; i += 1 ) {
        assertions_add_( as, array[ i ] );
    }
}

