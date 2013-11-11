// assertion-private.c

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


#include "assertion.h" // TestAssertion
#include "assertion-private.h"

#include <stdlib.h>
#include <stdio.h>


void test_assertion_print( TestAssertion const a,
                           FILE * const file,
                           char const * indent )
{
    fprintf( file, ( a.result == true ) ? "true" : "false" );
    fprintf( file, ":  %s", a.expr );
    if ( a.num_ids > 0 ) {
        fprintf( file, "\n%s%s%s(for ", indent, indent, indent );
        for ( size_t i = 0; i < a.num_ids; i += 1 ) {
            if ( i != 0 ) {
                fprintf( file, ", " );
            }
            TestAssertionId const id = a.ids[ i ];
            fprintf( file, "%s = %d", id.expr, id.value );
        }
        fprintf( file, ")" );
    }
    fprintf( file, "\n" );
}


void test_assertions_free( TestAssertion * const as )
{
    for ( size_t i = 0; !test_assertion_is_end( as[ i ] ); i += 1 ) {
        free( as[ i ].ids );
    }
    free( as );
}

