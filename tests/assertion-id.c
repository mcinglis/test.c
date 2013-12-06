// tests/assertion-id.c

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

#include <test.h>

#include <assertion-id.h> // AssertionId, assertion_id_*
#include <_common.h> // NELEM


static AssertionId const examples[] = {
    ASSERTION_ID_ARRAY_END,
    { .value = 0, .expr = "" },
    { .value = 0, .expr = "a" },
    { .value = 0, .expr = "ab" },
    { .value = 1, .expr = "" },
    { .value = 1, .expr = "a" },
    { .value = 1, .expr = "ab" },
    { .value = 2, .expr = "" },
    { .value = 2, .expr = "a" },
    { .value = 2, .expr = "ab" }
};


static
Assertions * all_examples_are_valid( void )
{
    Assertions * const as = assertions_empty();
    for ( size_t i = 0; i < NELEM( examples ); i += 1 ) {
        assertions_add( as, assertion_id_is_valid( examples[ i ] ), i );
    }
    return as;
}


static
Assertions * assertion_id_eq__works( void )
{
    Assertions * const as = assertions_empty();
    for ( size_t i = 0; i < NELEM( examples ); i += 1 ) {
        for ( size_t j = 0; j < NELEM( examples ); j += 1 ) {
            bool const eq_ij = assertion_id_eq( examples[ i ], examples[ j ] );
            bool const eq_ji = assertion_id_eq( examples[ j ], examples[ i ] );
            if ( i == j ) {
                assertions_add( as, eq_ij, i, j );
                assertions_add( as, eq_ji, i, j );
            } else {
                assertions_add( as, !eq_ij, i, j );
                assertions_add( as, !eq_ji, i, j );
            }
        }
    }
    return as;
}


static
Assertions * assertion_id_is_array_end__works( void )
{
    Assertions * const as = assertions(
        assertion_id_is_array_end( examples[ 0 ] )
    );
    for ( size_t i = 1; i < NELEM( examples ); i += 1 ) {
        assertions_add( as, !assertion_id_is_array_end( examples[ i ] ), i );
    }
    return as;
}


Test const assertion_id_tests[] = TEST_ARRAY(
    all_examples_are_valid,
    assertion_id_eq__works,
    assertion_id_is_array_end__works
);

