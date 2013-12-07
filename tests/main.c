// tests/main.c

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


#include <test.h> // Test, tests_return_val, tests_run


extern Test const assertion_id_tests[];
extern Test const assertion_ids_tests[];
extern Test const assertion_tests[];
extern Test const assertions_tests[];
extern Test const test_tests[];


int main( void )
{
    return tests_return_val(
        tests_run( "AssertionId", assertion_id_tests ),
        tests_run( "AssertionIds", assertion_ids_tests ),
        tests_run( "Assertion", assertion_tests ),
        tests_run( "Assertions", assertions_tests ),
        tests_run( "Test", test_tests )
    );
}

