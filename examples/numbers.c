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
#include <assert.h>

#include <test.h>
// Test, TEST_ARRAY, test*, Assertions, assertions*


struct Assertions * numbers( void )
{
    return assertions(
        3 + 8 == 11,
        4 * 4 > 10,
        3 * 3 == 8 || 2 * 2 == 4
    );
}


struct Assertions * strings( void )
{
    return assertions(
        strcmp( "any", "boolean" ) < 0,
        strlen( "expression" ) != 3,
        "works"[ 2 ] == 'x'
    );
}


struct Assertions * xs_is_increasing( void )
{
    int const xs[] = { 3, 14, 98, 34, 291, 498, 89, -1 };
    struct Assertions * const as = assertions_empty();
    for ( size_t i = 0; xs[ i + 1 ] != -1; i += 1 ) {
        assertions_add( as, xs[ i ] <= xs[ i + 1 ],
                            i, xs[ i ], xs[ i + 1 ] );
    }
    return as;
}


int main( void ) {
    tests_run( "example", ( struct Test[] ) TEST_ARRAY(
        numbers,
        strings,
        xs_is_increasing
    ) );
}

