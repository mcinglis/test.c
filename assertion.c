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


#include "assertion.h" // Assertion

#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "_common.h" // string_eq
#include "assertion-id.h" // AssertionId
#include "assertion-ids.h" // AssertionIds, assertion_ids_*


bool assertion_is_valid( Assertion const a )
{
    return a.expr != NULL
        && ( a.ids == NULL
          || assertion_ids_is_valid( *( a.ids ) ) );
}


void assertion_assert_valid( Assertion const a )
{
    assert( a.expr != NULL );
    if ( a.ids != NULL ) {
        assertion_ids_assert_valid( *( a.ids ) );
    }
}


Assertion * assertion_new_( struct assertion_new_options const o )
{
    Assertion * const a = malloc( sizeof ( Assertion ) );
    *a = ( Assertion ){
        .expr = o.expr,
        .result = o.result,
        .ids = assertion_ids_new( .array = o.ids )
    };
    return a;
}


Assertion * assertion_copy( Assertion const a )
{
    assertion_assert_valid( a );
    Assertion * const copy = malloc( sizeof a );
    *copy = ( Assertion ){
        .expr = a.expr,
        .result = a.result,
        .ids = ( a.ids == NULL ) ? assertion_ids_empty()
                                 : assertion_ids_copy( *( a.ids ) )
    };
    return copy;
}


void assertion_free( Assertion * const a )
{
    if ( a != NULL ) {
        assertion_assert_valid( *a );
        assertion_ids_free( a->ids );
        free( a );
    }
}


bool assertion_eq( Assertion const a1, Assertion const a2 )
{
    assertion_assert_valid( a1 );
    assertion_assert_valid( a2 );
    return a1.result == a2.result
        && string_eq( a1.expr, a2.expr )
        && ( a1.ids == a2.ids
          || ( !assertion_has_ids( a1 )
            && !assertion_has_ids( a2 ) )
          || ( a1.ids != NULL
            && a2.ids != NULL
            && assertion_ids_eq( *( a1.ids ), *( a2.ids ) ) ) );
}


bool assertion_has_ids( Assertion const a )
{
    assertion_assert_valid( a );
    return a.ids != NULL && !assertion_ids_is_empty( *( a.ids ) );
}


void assertion_print_( struct assertion_print_options const o )
{
    Assertion const a = o.assertion;
    assertion_assert_valid( a );
    FILE * const file = ( o.file == NULL ) ? stdout : o.file;
    char const * const ids_indent = ( o.ids_indent == NULL ) ? ""
                                                             : o.ids_indent;

    fprintf( file, "%s:  %s\n", ( a.result == true ) ? "true" : "false",
                                a.expr );
    if ( assertion_has_ids( a ) ) {
        fprintf( file, "%s", ids_indent );
        assertion_ids_print( .ids = *( a.ids ), .file = file );
    }
}


