// tests/assertions.c

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


#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <test.h>


static
Assertions * make_ex_assertions( void )
{
    return assertions(
        5 < 10,
        10 * 10 == 100,
        5 + 4 < 8,
        "test"[ 2 ] == 'x',
        6 / 3 == 2
    );
}


static
Assertions * assertions_get__nonnegative( void )
{
    Assertions * const ex = make_ex_assertions();
    Assertions * const as = assertions(
        assertion_eq( *assertions_get( *ex, 0 ), ( Assertion ){
            .expr = "5 < 10",
            .result = true
        } ),
        assertion_eq( *assertions_get( *ex, 1 ), ( Assertion ){
            .expr = "10 * 10 == 100",
            .result = true
        } ),
        assertion_eq( *assertions_get( *ex, 2 ), ( Assertion ){
            .expr = "5 + 4 < 8",
            .result = false
        } ),
        assertion_eq( *assertions_get( *ex, 3 ), ( Assertion ){
            .expr = "\"test\"[ 2 ] == 'x'",
            .result = false
        } ),
        assertion_eq( *assertions_get( *ex, 4 ), ( Assertion ){
            .expr = "6 / 3 == 2",
            .result = true
        } )
    );
    assertions_free( ex );
    return as;
}


static
Assertions * assertions_get__negative( void )
{
    Assertions * const ex = make_ex_assertions();
    Assertions * const as = assertions(
        assertion_eq( *assertions_get( *ex, -5 ), ( Assertion ){
            .expr = "5 < 10",
            .result = true
        } ),
        assertion_eq( *assertions_get( *ex, -4 ), ( Assertion ){
            .expr = "10 * 10 == 100",
            .result = true
        } ),
        assertion_eq( *assertions_get( *ex, -3 ), ( Assertion ){
            .expr = "5 + 4 < 8",
            .result = false
        } ),
        assertion_eq( *assertions_get( *ex, -2 ), ( Assertion ){
            .expr = "\"test\"[ 2 ] == 'x'",
            .result = false
        } ),
        assertion_eq( *assertions_get( *ex, -1 ), ( Assertion ){
            .expr = "6 / 3 == 2",
            .result = true
        } )
    );
    assertions_free( ex );
    return as;
}


static
Assertions * assertions_increase_capacity__works( void )
{
    // Given a sequence of assertions:
    Assertions * const new = assertions( 1 == 1, 2 == 2, 3 == 3 );
    Assertions * const old = assertions_copy( *new );

    // When we increase the capacity of that sequence:
    assertions_increase_capacity( new );

    // Then its capacity should be greater than what it was, and it
    // should still contain the same assertions:
    assertions_assert_valid( *new );
    Assertions * const as = assertions(
        old->capacity < new->capacity,
        assertions_eq( *old, *new )
    );

    assertions_free( new );
    return as;
}


static
Assertions * assertions_decrease_capacity__no_trim( void )
{
    // Given a sequence of assertions whose capacity is more than or
    // equal to twice its number of assertions:
    Assertions * const new = assertions_new(
        .capacity = 8,
        .array = ( Assertion[] ) ASSERTION_ARRAY( 1 == 1, 2 == 2, 3 == 3 )
    );
    assert( new->capacity >= new->size * 2 );
    Assertions * const old = assertions_copy( *new );

    // When we halve the capacity of that given sequence:
    assertions_decrease_capacity( new );

    // Then its capacity should be half of what it was, but it should
    // still have the same assertions it did previously:
    assertions_assert_valid( *new );
    Assertions * const as = assertions(
        new->capacity == old->capacity / 2,
        assertions_eq( *new, *old )
    );

    assertions_free( new );
    assertions_free( old );
    return as;
}


static
Assertions * assertions_add__up_to_capacity( void )
{
    // Given a sequence of assertions:
    Assertions * const ex = make_ex_assertions();
    size_t const capacity = ex->size;

    // When we allocate a new sequence with a `capacity` equal to the
    // `size` of the given sequence, and add every element from that
    // sequence:
    Assertions * const new = assertions_new( .capacity = capacity );
    for ( size_t i = 0; i < capacity; i += 1 ) {
        assertions_add_( new, *assertions_get( *ex, i ) );
    }

    // Then the new sequence should still have the same `capacity` (it
    // should not have needed to increase it), and it should be equal to
    // the given sequence:
    assertions_assert_valid( *new );
    Assertions * const as = assertions(
        new->capacity == capacity,
        assertions_eq( *new, *ex )
    );

    assertions_free( ex );
    assertions_free( new );
    return as;
}


static
Assertions * assertions_add__beyond_capacity( void )
{
    // Given a sequence of assertions:
    Assertions * const ex = make_ex_assertions();
    size_t const capacity = ex->size;
    int const repeat = 3;

    // When we allocate a new sequence with a `capacity` equal to the
    // `size` of the given sequence, and add every element from that
    // sequence `repeat` times:
    Assertions * const new = assertions_new( .capacity = capacity );
    for ( size_t i = 0; i < capacity; i += 1 ) {
        for ( int j = 0; j < repeat; j += 1 ) {
            assertions_add_( new, *assertions_get( *ex, i ) );
        }
    }

    // Then the new sequence should have a larger `capacity` than what
    // it was created with (it should have needed to increase it), and
    // it should have the right elements.
    assertions_assert_valid( *new );
    Assertions * const as = assertions(
        new->capacity > capacity,
        new->size == ex->size * repeat
    );
    for ( size_t i = 0; i < ex->size; i += repeat ) {
        for ( int j = 0; j < repeat; j += 1 ) {
            assertions_add( as,
                assertion_eq( *assertions_get( *new, i + j ),
                              *assertions_get( *ex, i / repeat ) ),
                i, j
            );
        }
    }

    assertions_free( ex );
    assertions_free( new );
    return as;
}


Test const assertions_tests[] = TEST_ARRAY(
    assertions_get__nonnegative,
    assertions_get__negative,
    assertions_increase_capacity__works,
    assertions_decrease_capacity__no_trim,
    assertions_add__up_to_capacity,
    assertions_add__beyond_capacity
);


