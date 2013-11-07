// tests/assertion.c

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

#include <test.h>
// Test, TestAssertion, TESTS, test_assertion, test_assertion_eq


#define FREE_AFTER_SIZE 3


static
void * before_each( void )
{
    // Provide an array of assertion array pointers to each test, so
    // they can store references to their allocated assertion arrays
    // to be freed by `after_each`.
    return calloc( FREE_AFTER_SIZE, sizeof( TestAssertion * ) );
}


static
void after_each( void * const data )
{
    TestAssertion * * const ass = data;
    for ( int i = 0; i < FREE_AFTER_SIZE; i += 1 ) {
        free( ass[ i ] );
    }
    free( ass );
}


static
TestAssertion * test_assertion_eq__is_correct( void * const data )
{
    return test_assert(
        test_assertion_eq(
            ( TestAssertion ){ .expr = NULL },
            ( TestAssertion ){ .expr = NULL }
        ),
        test_assertion_eq(
            ( TestAssertion ){ .expr = "1 + 1 == 2", .result = true },
            ( TestAssertion ){ .expr = "1 + 1 == 2", .result = true }
        ),
        test_assertion_eq(
            ( TestAssertion ){ .expr = "anything", .result = false },
            ( TestAssertion ){ .expr = "anything", .result = false }
        ),
        !test_assertion_eq( // Different expressions => not equal
            ( TestAssertion ){ .expr = "1 + 3 == 4", .result = true },
            ( TestAssertion ){ .expr = "1 + 4 == 5", .result = true }
        ),
        !test_assertion_eq( // Different results => not equal
            ( TestAssertion ){ .expr = "3 / 3 == 1", .result = false },
            ( TestAssertion ){ .expr = "3 / 3 == 1", .result = true }
        ),
        test_assertion_eq(
            ( TestAssertion ){ .expr = "x + x == 4", .result = true,
                               .id_expr = "x", .id = 2 },
            ( TestAssertion ){ .expr = "x + x == 4", .result = true,
                               .id_expr = "x", .id = 2 }
        ),
        test_assertion_eq(
            ( TestAssertion ){ .expr = "x + x == 4", .result = true,
                               .id_expr = NULL, .id = 3 },
            ( TestAssertion ){ .expr = "x + x == 4", .result = true,
                               .id_expr = NULL, .id = 3 }
        ),
        !test_assertion_eq( // Different id values => not equal
            ( TestAssertion ){ .expr = "x + x == 4", .result = true,
                               .id_expr = "x", .id = 2 },
            ( TestAssertion ){ .expr = "x + x == 4", .result = true,
                               .id_expr = "x", .id = 3 }
        ),
        !test_assertion_eq( // Different id expressions => not equal
            ( TestAssertion ){ .expr = "x + x == 4", .result = true,
                               .id_expr = "x", .id = 2 },
            ( TestAssertion ){ .expr = "x + x == 4", .result = true,
                               .id_expr = "y", .id = 2 }
        )
    );
}


static
TestAssertion * test_assert__gives_right_assertions( void * const data )
// This indirectly tests `test_assertions_alloc`, because `test_assert`
// is just a macro around it to provide a nicer interface.
{
    TestAssertion * * const free_after = data;
    TestAssertion * const as = test_assert( 1 == 1,
                                            4 / 2 == 3,
                                            strcmp( "hello", "hello" ) == 0 );
    free_after[ 0 ] = as;
    return test_assert(
        test_assertion_eq( as[ 0 ], ( TestAssertion ){
            .expr = "1 == 1", .result = true
        } ),
        test_assertion_eq( as[ 1 ], ( TestAssertion ){
            .expr = "4 / 2 == 3", .result = false
        } ),
        test_assertion_eq( as[ 2 ], ( TestAssertion ){
            .expr = "strcmp( \"hello\", \"hello\" ) == 0", .result = true
        } ),
        test_assertion_eq( as[ 3 ], ( TestAssertion ){ .expr = NULL } )
    );
}



static
TestAssertion * TEST_REQUIRE__returns_only_when_false__f( void )
{
    for ( int i = 0; i < 10; i += 1 ) {
        TEST_REQUIRE( i != 7, i );
    }
    return NULL;
}


static
TestAssertion * TEST_REQUIRE__returns_only_when_false__g( void )
{
    for ( int i = 0; i < 10; i += 1 ) {
        TEST_REQUIRE( i != 11, i );
    }
    return NULL;
}


static
TestAssertion * TEST_REQUIRE__returns_only_when_false( void * const data )
{
    TestAssertion * * const free_after = data;
    TestAssertion * const f_as = TEST_REQUIRE__returns_only_when_false__f();
    TestAssertion * const g_as = TEST_REQUIRE__returns_only_when_false__g();
    free_after[ 0 ] = f_as;
    return test_assert(
        test_assertion_eq( f_as[ 0 ], ( TestAssertion ){
            .expr = "i != 7", .result = false, .id_expr = "i", .id = 7
        } ),
        test_assertion_eq( f_as[ 1 ], ( TestAssertion ){ .expr = NULL } ),
        g_as == NULL
    );
}


Test const assertion_tests[] = TESTS_FIX( before_each, after_each,
    test_assertion_eq__is_correct,
    test_assert__gives_right_assertions,
    TEST_REQUIRE__returns_only_when_false
);

