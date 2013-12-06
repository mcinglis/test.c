// assertion-ids.c

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


#include "assertion-ids.h" // AssertionIds

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "_common.h" // string_eq, MIN
#include "assertion-id.h" // AssertionId, assertion_id_eq


size_t const assertion_ids_initial_capacity = 4;


static
bool array_is_null_iff_capacity_is_zero( AssertionIds const ids )
// Checks an invariant condition.
{
    return ( ids.capacity == 0 && ids.array == NULL )
        || ( ids.capacity != 0 && ids.array != NULL );
}


static
bool all_elements_are_valid( AssertionIds const ids )
// Checks an invariant condition.
{
    for ( size_t i = 0; i < ids.size; i += 1 ) {
        if ( !assertion_id_is_valid( ids.array[ i ] ) ) {
            return false;
        }
    }
    return true;
}


bool assertion_ids_is_valid( AssertionIds const ids )
{
    return ids.size <= ids.capacity
        && array_is_null_iff_capacity_is_zero( ids )
        && all_elements_are_valid( ids );
}


void assertion_ids_assert_valid( AssertionIds const ids )
{
    assert( ids.size <= ids.capacity );
    assert( array_is_null_iff_capacity_is_zero( ids ) );
    // To get better assertion errors:
    for ( size_t i = 0; i < ids.size; i += 1 ) {
        assertion_id_assert_valid( ids.array[ i ] );
    }
}


AssertionIds * assertion_ids_new_( struct assertion_ids_new_options const o )
{
    AssertionId const * const array = o.array;
    size_t const capacity =
        ( o.capacity == 0 ) ? assertion_ids_initial_capacity : o.capacity;

    AssertionIds * const ids = malloc( sizeof ( AssertionIds ) );
    *ids = ( AssertionIds ){
        .size = 0,
        .capacity = capacity,
        .array = malloc( capacity * sizeof ( AssertionId ) )
    };
    if ( array != NULL ) {
        assertion_ids_add_all( ids, array );
    }
    return ids;
}


AssertionIds * assertion_ids_empty( void )
{
    return assertion_ids_new( .capacity = assertion_ids_initial_capacity );
}


AssertionIds * assertion_ids_copy( AssertionIds const ids )
{
    assertion_ids_assert_valid( ids );

    AssertionIds * const copy = assertion_ids_new( .capacity = ids.capacity );
    for ( size_t i = 0; i < ids.size; i += 1 ) {
        assertion_ids_add( copy, assertion_ids_get( ids, i ) );
    }
    return copy;
}


void assertion_ids_free( AssertionIds * const ids )
{
    if ( ids != NULL ) {
        assertion_ids_assert_valid( *ids );
        free( ids->array );
        free( ids );
    }
}


bool assertion_ids_eq( AssertionIds const ids1, AssertionIds const ids2 )
{
    assertion_ids_assert_valid( ids1 );
    assertion_ids_assert_valid( ids2 );

    if ( ids1.size != ids2.size ) {
        return false;
    }
    for ( size_t i = 0; i < ids1.size; i += 1 ) {
        if ( !assertion_id_eq( assertion_ids_get( ids1, i ),
                               assertion_ids_get( ids2, i ) ) ) {
            return false;
        }
    }
    return true;
}


bool assertion_ids_eq_array( AssertionIds const ids,
                             AssertionId const * const array )
{
    assertion_ids_assert_valid( ids );
    AssertionIds * const new = assertion_ids_new( .array = array );
    bool const eq = assertion_ids_eq( ids, *new );
    assertion_ids_free( new );
    return eq;
}


void assertion_ids_print_( struct assertion_ids_print_options const o )
{
    AssertionIds const ids = o.ids;
    assertion_ids_assert_valid( ids );
    FILE * const file = ( o.file == NULL ) ? stdout : o.file;

    fprintf( file, "(for " );
    for ( size_t i = 0; i < ids.size; i += 1 ) {
        AssertionId const id = assertion_ids_get( ids, i );
        fprintf( file, "%s%s = %d", ( i > 0 ) ? ", " : "", id.expr, id.value );
    }
    fprintf( file, ")\n" );

}


bool assertion_ids_is_empty( AssertionIds const ids )
{
    assertion_ids_assert_valid( ids );
    return ids.size == 0;
}


void assertion_ids_increase_capacity( AssertionIds * const ids )
{
    assert( ids != NULL );
    assertion_ids_assert_valid( *ids );

    ids->capacity *= 2;
    ids->array = realloc( ids->array, ids->capacity * sizeof ( AssertionId ) );
}


void assertion_ids_decrease_capacity( AssertionIds * const ids )
{
    assert( ids != NULL );
    assertion_ids_assert_valid( *ids );

    if ( ids->capacity == 0 ) {
        return;
    }
    ids->capacity /= 2;
    // Note: we may be decreasing the `capacity` below the current
    // `size`. If there's a change to allocate the elements of the
    // array, then you should free those pointers we'd be losing here.
    // As it is, the elements aren't allocated, so we can simply
    // decrease the `size` as needed.
    ids->size = MIN( ids->size, ids->capacity );
    ids->array = realloc( ids->array, ids->capacity * sizeof ( AssertionId ) );
    if ( ids->capacity == 0 ) {
        // `realloc` may not return `NULL` if the given `size` is `0`,
        // but it will certainly free the given pointer.
        ids->array = NULL;
    }
}


AssertionId assertion_ids_get( AssertionIds const ids, long long const i )
{
    assertion_ids_assert_valid( ids );
    long long const lsize = ids.size;
    assert( -lsize <= i && i < lsize );

    return ids.array[ ( i < 0 ) ? ( lsize + i ) : i ];
}


void assertion_ids_add( AssertionIds * const ids, AssertionId const id )
{
    assert( ids != NULL );
    assertion_ids_assert_valid( *ids );
    assertion_id_assert_valid( id );

    if ( ids->size == ids->capacity ) {
        assertion_ids_increase_capacity( ids );
    }
    ids->array[ ids->size ] = id;
    ids->size += 1;
}


void assertion_ids_add_all( AssertionIds * const ids,
                            AssertionId const * const array )
{
    assert( ids != NULL );
    assertion_ids_assert_valid( *ids );
    assert( array != NULL );

    for ( size_t i = 0; !assertion_id_is_array_end( array[ i ] ); i += 1 ) {
        assertion_ids_add( ids, array[ i ] );
    }
}

