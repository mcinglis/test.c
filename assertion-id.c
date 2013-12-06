// assertion-id.c

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


#include "assertion-id.h" // AssertionId

#include <stdlib.h>
#include <assert.h>

#include "_common.h" // string_eq


AssertionId const assertion_id_array_end = ASSERTION_ID_ARRAY_END;


bool assertion_id_is_valid( AssertionId const id )
{
    return id.expr != NULL;
}


void assertion_id_assert_valid( AssertionId const id )
{
    assert( id.expr != NULL );
}


bool assertion_id_eq( AssertionId const id1, AssertionId const id2 )
{
    assertion_id_assert_valid( id1 );
    assertion_id_assert_valid( id2 );
    return id1.value == id2.value
        && string_eq( id1.expr, id2.expr );
}


bool assertion_id_is_array_end( AssertionId const id )
{
    return assertion_id_eq( id, assertion_id_array_end );
}

