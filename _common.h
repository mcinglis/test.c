// _common.h
// Functions used throughout Test.c.

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


#ifndef INCLUDED_TESTC__COMMON_H
#define INCLUDED_TESTC__COMMON_H


#include <stdbool.h>


#define MAX( A, B ) \
    ( ( A ) > ( B ) ? ( A ) : ( B ) )


#define MIN( A, B ) \
    ( ( A ) < ( B ) ? ( A ) : ( B ) )


#define NELEM( XS ) \
    ( ( sizeof XS ) / ( sizeof XS[ 0 ] ) )


// Returns true if both of the given pointers point to the same array,
// or are both null, or if `strcmp( s1, s2 ) == 0`.
bool string_eq( char const * const s1, char const * const s2 );


#endif // ifndef INCLUDED_TESTC__COMMON_H

