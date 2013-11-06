// examples/sum.c

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


#include "test.h"
// Test, TestAssertion, test_assert, TEST_REQUIRE, tests_run


#define NELEM( XS ) ( sizeof( XS ) / sizeof( XS[ 0 ] ) )


int sum( int const * const xs, int const n )
{
    int s = 0;
    for ( int i = 0; i < n; i += 1 ) {
        s += xs[ i ];
    }
    return s;
}


TestAssertion * sum_declared( void * const data )
{
    int xs[] = { 1, 2, 3 };
    int ys[] = { 4, 5, 6, 7, 8 };
    return test_assert( sum( xs, NELEM( xs ) ) == 6,
                        sum( ys, NELEM( ys ) ) == 30 );
}


TestAssertion * sum_literal( void * const data )
{
    return test_assert( sum( ( int[] ){ 0 }, 0 ) == 0,
                        sum( ( int[] ){ -1, 8 }, 2 ) == 7,
                        sum( ( int[] ){ 1, 0, 1 }, 3 ) == 2 );
}


int main( void ) {
    tests_run( "sum", ( Test[] ) TESTS(
        sum_declared,
        sum_literal
    ) );
}

