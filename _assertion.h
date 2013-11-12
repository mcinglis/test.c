// _assertion.h

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


#ifndef INCLUDED_TESTC__ASSERTION_H
#define INCLUDED_TESTC__ASSERTION_H


#include <stdio.h>

#include "assertion.h" // TestAssertion


void test_assertion_print( TestAssertion const a,
                           FILE * const file,
                           char const * indent );


void test_assertions_free( TestAssertion * const as );


#endif // ifndef INCLUDED_TESTC__ASSERTION_H

