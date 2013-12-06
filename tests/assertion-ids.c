// tests/assertion-ids.c

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


#include <string.h>
#include <assert.h>

#include <test.h>

#include <assertion-ids.h> // AssertionIds, assertion_ids_*
#include <_common.h> // NELEM


#define EXAMPLE_IDSS \
    { \
        assertion_ids_empty(), \
        assertion_ids( 1 ), \
        assertion_ids( 2 ), \
        assertion_ids( 1, 2 ), \
        assertion_ids( 2, 1 ), \
        assertion_ids( 1, 2, 3 ), \
        assertion_ids( 1, 3, 2 ), \
        assertion_ids( 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ) \
    }


static
Assertions * assertion_ids_new__zero_capacity( void )
{
    // When:
    AssertionIds * const ids = assertion_ids_new( .capacity = 0 );
    // Then:
    assertion_ids_assert_valid( *ids );
    Assertions * const as = assertions(
        ids->size == 0,
        ids->capacity == assertion_ids_initial_capacity
    );

    assertion_ids_free( ids );
    return as;
}


static
Assertions * assertion_ids_new__nonzero_capacity( void )
{
    // Given:
    size_t const capacity = 4;
    // When:
    AssertionIds * const ids = assertion_ids_new( .capacity = capacity );
    // Then:
    assertion_ids_assert_valid( *ids );
    Assertions * const as = assertions(
        ids->size == 0,
        ids->capacity == capacity
    );

    assertion_ids_free( ids );
    return as;
}


static
Assertions * assertion_ids_new__array_and_larger_capacity( void )
{
    // Given:
    AssertionId const array[] = ASSERTION_ID_ARRAY( 1, 2, 3 );
    size_t const array_size = NELEM( array ) - 1; // don't count terminator
    size_t const capacity = 4;
    assert( array_size <= capacity );
    // When:
    AssertionIds * const ids = assertion_ids_new( .array = array,
                                                  .capacity = capacity );
    // Then:
    assertion_ids_assert_valid( *ids );
    Assertions * const as = assertions(
        ids->size == array_size,
        ids->capacity == capacity,
        assertion_ids_eq_array( *ids, array )
    );

    assertion_ids_free( ids );
    return as;
}


static
Assertions * assertion_ids_new__array_and_smaller_capacity( void )
{
    // Given:
    AssertionId const array[] = ASSERTION_ID_ARRAY( 1, 2, 3, 4, 5 );
    size_t const array_size = NELEM( array ) - 1; // don't count terminator
    size_t const capacity = 4;
    assert( array_size > capacity );
    // When:
    AssertionIds * const ids = assertion_ids_new( .array = array,
                                                  .capacity = capacity );
    // Then:
    assertion_ids_assert_valid( *ids );
    Assertions * const as = assertions(
        ids->size == array_size,
        ids->capacity > capacity,
        assertion_ids_eq_array( *ids, array )
    );

    assertion_ids_free( ids );
    return as;
}


static
Assertions * assertion_ids_empty__works( void )
{
    // When:
    AssertionIds * const ids = assertion_ids_empty();
    // Then:
    assertion_ids_assert_valid( *ids );
    Assertions * const as = assertions(
        ids->size == 0,
        ids->capacity == assertion_ids_initial_capacity
    );

    assertion_ids_free( ids );
    return as;
}


static
Assertions * example_idss_are_valid( void )
{
    AssertionIds * const idss[] = EXAMPLE_IDSS;
    Assertions * const as = assertions_empty();
    for ( size_t i = 0; i < NELEM( idss ); i += 1 ) {
        assertions_add( as, assertion_ids_is_valid( *idss[ i ] ), i );
    }
    return as;
}


static
Assertions * assertion_ids_copy__gives_equal( void )
{
    // Given:
    AssertionIds * const idss[] = EXAMPLE_IDSS;

    Assertions * const as = assertions_empty();
    for ( size_t i = 0; i < NELEM( idss ); i += 1 ) {
        // When:
        AssertionIds * const copy = assertion_ids_copy( *idss[ i ] );
        // Then:
        assertion_ids_assert_valid( *copy );
        assertions_add( as, assertion_ids_eq( *copy, *idss[ i ] ), i );

        assertion_ids_free( copy );
        assertion_ids_free( idss[ i ] );
    }

    return as;
}


static
Assertions * assertion_ids_eq__works( void )
{
    // Given:
    AssertionIds * const idss[] = EXAMPLE_IDSS;

    Assertions * const as = assertions_empty();
    for ( size_t i = 0; i < NELEM( idss ); i += 1 ) {
        for ( size_t j = 0; j < NELEM( idss ); j += 1 ) {
            // When:
            bool const eq_ij = assertion_ids_eq( *idss[ i ], *idss[ j ] );
            bool const eq_ji = assertion_ids_eq( *idss[ j ], *idss[ i ] );
            // Then:
            if ( i == j ) {
                assertions_add( as, eq_ij, i, j );
                assertions_add( as, eq_ji, i, j );
            } else {
                assertions_add( as, !eq_ij, i, j );
                assertions_add( as, !eq_ji, i, j );
            }
        }
    }

    for ( size_t i = 0; i < NELEM( idss ); i += 1 ) {
        assertion_ids_free( idss[ i ] );
    }
    return as;
}


static
Assertions * assertion_ids_eq_array__works( void )
{
    // Given:
    AssertionIds * const idss[] = {
        assertion_ids_empty(),
        assertion_ids( 1 ),
        assertion_ids( 2 ),
        assertion_ids( 1, 2 ),
        assertion_ids( 2, 1 ),
        assertion_ids( 1, 2, 3 ),
        assertion_ids( 1, 2, 4 )
    };
    AssertionId * const arrays[] = {
        ( AssertionId[] ) { ASSERTION_ID_ARRAY_END },
        ( AssertionId[] ) ASSERTION_ID_ARRAY( 1 ),
        ( AssertionId[] ) ASSERTION_ID_ARRAY( 2 ),
        ( AssertionId[] ) ASSERTION_ID_ARRAY( 1, 2 ),
        ( AssertionId[] ) ASSERTION_ID_ARRAY( 2, 1 ),
        ( AssertionId[] ) ASSERTION_ID_ARRAY( 1, 2, 3 ),
        ( AssertionId[] ) ASSERTION_ID_ARRAY( 1, 2, 4 )
    };
    assert( NELEM( idss ) == NELEM( arrays ) );

    Assertions * const as = assertions_empty();
    for ( size_t i = 0; i < NELEM( idss ); i += 1 ) {
        for ( size_t j = 0; j < NELEM( idss ); j += 1 ) {
            // When:
            bool const eq = assertion_ids_eq_array( *idss[ i ], arrays[ j ] );
            // Then:
            if ( i == j ) {
                assertions_add( as, eq, i, j );
            } else {
                assertions_add( as, !eq, i, j );
            }
        }
    }

    for ( size_t i = 0; i < NELEM( idss ); i += 1 ) {
        assertion_ids_free( idss[ i ] );
    }
    return as;
}


static
Assertions * assertion_ids_is_empty__works( void )
{
    // Given:
    AssertionIds * const idss[] = EXAMPLE_IDSS;

    Assertions * const as = assertions( assertion_ids_is_empty( *idss[ 0 ] ) );
    for ( size_t i = 1; i < NELEM( idss ); i += 1 ) {
        assertions_add( as, !assertion_ids_is_empty( *idss[ i ] ), i );
    }

    for ( size_t i = 0; i < NELEM( idss ); i += 1 ) {
        assertion_ids_free( idss[ i ] );
    }
    return as;
}


static
Assertions * assertion_ids_increase_capacity__works( void )
{
    // Given:
    AssertionIds * const idss[] = EXAMPLE_IDSS;

    Assertions * const as = assertions_empty();
    for ( size_t i = 0; i < NELEM( idss ); i += 1 ) {
        AssertionIds * const ids = idss[ i ];
        // When:
        AssertionIds * const old = assertion_ids_copy( *ids );
        assertion_ids_increase_capacity( ids );
        // Then:
        assertion_ids_assert_valid( *ids );
        assertions_add( as, old->capacity < ids->capacity, i );
        assertions_add( as, assertion_ids_eq( *old, *ids ), i );

        assertion_ids_free( ids );
        assertion_ids_free( old );
    }

    return as;
}


static
Assertions * assertion_ids_decrease_capacity__no_trim( void )
{
    // Given:
    AssertionIds * const idss[] = {
        assertion_ids_new( .capacity = 0 ),
        assertion_ids_empty(),
        assertion_ids( 1 ),
        assertion_ids( 2 ),
        assertion_ids( 1, 2 ),
        assertion_ids( 2, 1 )
    };

    Assertions * const as = assertions_empty();
    for ( size_t i = 0; i < NELEM( idss ); i += 1 ) {
        AssertionIds * const ids = idss[ i ];
        assert( ids->capacity >= ids->size * 2 );
        // When:
        AssertionIds * const old = assertion_ids_copy( *ids );
        assertion_ids_decrease_capacity( ids );
        // Then:
        assertion_ids_assert_valid( *ids );
        if ( old->capacity == 0 ) {
            assertions_add( as, ids->capacity == old->capacity, 0 );
        } else {
            assertions_add( as, ids->capacity == old->capacity / 2, i );
        }
        assertions_add( as, assertion_ids_eq( *old, *ids ), i );

        assertion_ids_free( ids );
        assertion_ids_free( old );
    }

    return as;
}


Test const assertion_ids_tests[] = TEST_ARRAY(
    assertion_ids_new__zero_capacity,
    assertion_ids_new__nonzero_capacity,
    assertion_ids_new__array_and_larger_capacity,
    assertion_ids_new__array_and_smaller_capacity,
    assertion_ids_empty__works,
    example_idss_are_valid,
    assertion_ids_copy__gives_equal,
    assertion_ids_eq__works,
    assertion_ids_eq_array__works,
    assertion_ids_is_empty__works,
    assertion_ids_increase_capacity__works,
    assertion_ids_decrease_capacity__no_trim
);

