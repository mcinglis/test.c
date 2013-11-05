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


TestAssertion * addition_works( void )
{
    return test_assert(
        2 + 2 == 4,
        ( 1 + 1 == 3 ) || ( 9 - 3 == 6 )
    );
}


TestAssertion * multiplication_works( void )
{
    return test_assert(
        1 * 5 == 1,
        9 * 2 == 18,
        3 * 4 != 12
    );
}


TestAssertion * some_numbers_dont_exist( void )
{
    for ( int x = 0; x < 100; x += 1 ) {
        TEST_REQUIRE( x != 17 && x != 42, x );
    }
    return NULL;
}


Test const arithmetic_tests[] = TESTS(
    addition_works,
    multiplication_works,
    some_numbers_dont_exist
);


int main( void ) {
    tests_run( "arithmetic", arithmetic_tests );
}

