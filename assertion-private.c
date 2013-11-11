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


void test_assertion_print( FILE * file, TestAssertion const a )
{
    fprintf( file, ( a.result == true ) ? "true" : "false" );
    for ( size_t i = 0; i < a.num_ids; i += 1 ) {
        fprintf( file, ( i == 0 ) ? " for " : ", " );
        fprintf( file, "%s = %d", a.ids[ i ].expr, a.ids[ i ].value );
    }
    fprintf( file, ":  %s\n", a.expr );
}


void test_assertions_free( TestAssertion * const as )
{
    for ( size_t i = 0; !test_assertion_is_end( as[ i ] ); i += 1 ) {
        free( as[ i ].ids );
    }
    free( as );
}

