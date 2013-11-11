// examples/numbers.c

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

#include <test.h>
// Test, TestAssertion, test_assert, TEST_REQUIRE, tests_run


void * before_each( void )
{
    int const xs[] = { 2, 4, 8, 5, 3 };
    int * const mem = malloc( sizeof( xs ) );
    memcpy( mem, xs, sizeof( xs ) );
    return mem;
}


void after_each( void * const data )
{
    int * const xs = data;
    free( xs );
}


struct TestAssertion * addition_works( void * const data )
{
    return test_assert( 3 + 8 == 11 );
}


struct TestAssertion * multiplication_works( void * const data )
{
    int * const xs = data;
    xs[ 2 ] = 3;
    return test_assert( 2 * 2 == 5,
                        3 * xs[ 2 ] == 9,
                        xs[ 1 ] * 4 != 16 );
}


struct TestAssertion * xs_is_increasing( void * const data )
{
    int const * const xs = data;
    for ( int i = 0; i < 4; i += 1 ) {
        TEST_REQUIRE( xs[ i ] <= xs[ i + 1 ],
                      i, xs[ i ], xs[ i + 1 ] );
    }
    return NULL;
}


struct Test const number_tests[] = TESTS_FIX( before_each, after_each,
    addition_works,
    multiplication_works,
    xs_is_increasing
);


int main( void ) {
    tests_run( "number", number_tests );
}

