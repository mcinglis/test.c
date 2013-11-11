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


#include "assertion.h" // TestAssertion, TestAssertionId

#include <string.h>
#include <assert.h>

#include "common.h" // string_eq


static
bool test_assertion_id_eq( TestAssertionId const id1,
                           TestAssertionId const id2 )
{
    return id1.value == id2.value
        && string_eq( id1.expr, id2.expr );
}


static
bool test_assertion_ids_eq( TestAssertion const a1,
                            TestAssertion const a2 )
{
    if ( a1.num_ids != a2.num_ids ) {
        return false;
    }
    if ( a1.ids == a2.ids ) {
        return true;
    }
    for ( size_t i = 0; i < a1.num_ids; i += 1 ) {
        if ( !test_assertion_id_eq( a1.ids[ i ], a2.ids[ i ] ) ) {
            return false;
        }
    }
    return true;
}


bool test_assertion_eq( TestAssertion const a1, TestAssertion const a2 )
{
    return a1.result == a2.result
        && string_eq( a1.expr, a2.expr )
        && test_assertion_ids_eq( a1, a2 );
}


bool test_assertion_is_end( TestAssertion const a )
{
    return test_assertion_eq( a, ( TestAssertion ) TEST_ASSERTIONS_END );
}


static
size_t test_assertions_size( TestAssertion const * const as )
{
    assert( as != NULL );
    int len = 0;
    for ( int i = 0; !test_assertion_is_end( as[ i ] ); i += 1 ) {
        len += 1;
    }
    return len + 1;
}


TestAssertion * test_assertions_alloc( TestAssertion const * const as )
{
    assert( as != NULL );
    size_t const as_bytes = test_assertions_size( as )
                            * sizeof( TestAssertion );
    TestAssertion * const alloc = malloc( as_bytes );
    memcpy( alloc, as, as_bytes );
    // Copy the identifications of each of the given assertions.
    for ( size_t i = 0; !test_assertion_is_end( as[ i ] ); i += 1 ) {
        if ( as[ i ].ids != NULL ) {
            // If the first identification expression is a literal `0`,
            // don't give the allocated assertion any identifications.
            if ( strcmp( as[ i ].ids[ 0 ].expr, "0" ) == 0 ) {
                alloc[ i ].ids = NULL;
                alloc[ i ].num_ids = 0;
            } else {
                size_t const ids_bytes = as[ i ].num_ids
                                         * sizeof( TestAssertionId );
                alloc[ i ].ids = malloc( ids_bytes );
                memcpy( alloc[ i ].ids, as[ i ].ids, ids_bytes );
            }
        }
    }
    return alloc;
}

