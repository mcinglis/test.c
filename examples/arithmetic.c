// examples/arithmetic.c

// Copyright (C) 2013  Malcolm Inglis
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


#include "test.h"
// Test, TestAssertion, test_assert, TEST_REQUIRE, tests_run


void * before_each( void )
{
    int * const x = malloc( sizeof( *x ) );
    *x = 2;
    return x;
}


void after_each( void * const data )
{
    free( data );
}


TestAssertion * addition_works( void * const data )
{
    int * const x = data;
    *x = 3;
    return test_assert(
        *x + *x == 6,
        ( 1 + 1 == 3 ) || ( 9 - 3 == 6 )
    );
}


TestAssertion * multiplication_works( void * const data )
{
    int const * const x = data;
    return test_assert(
        *x * 5 == 5,
        9 * *x == 18,
        3 * 4 != 12
    );
}


TestAssertion * some_numbers_dont_exist( void * const data )
{
    for ( int n = 0; n < 100; n += 1 ) {
        TEST_REQUIRE( n != 17 && n != 42, n );
    }
    return NULL;
}


Test const arithmetic_tests[] = TESTS_FIX( before_each, after_each,
    addition_works,
    multiplication_works,
    some_numbers_dont_exist
);


int main( void ) {
    tests_run( "arithmetic", arithmetic_tests );
}

