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

#include <_common.h> // NELEM


static
Assertion * * make_ex_assertions( void )
{
    int const x = 67;
    int const y = 67;
    Assertion * array[] = {
        assertion_new( 123 < 456, 0 ),
        assertion_new( 123 < 456, x ),
        assertion_new( 123 < 456, y ),
        assertion_new( 123 < 456, x, y ),
        assertion_new( 123 < 456, y, x ),
        assertion_new( 1 + 1 == 3, 0 ),
        assertion_new( 1 + 1 == 3, x ),
        assertion_new( 1 + 1 == 3, y ),
        assertion_new( 1 + 1 == 3, x, y ),
        assertion_new( 1 + 1 == 3, y, x ),
        NULL
    };
    Assertion * * const mem = malloc( sizeof array );
    memcpy( mem, array, sizeof array );
    return mem;
}


static
void free_ex_assertions( Assertion * * const array )
{
    for ( size_t i = 0; array[ i ] != NULL; i += 1 ) {
        assertion_free( array[ i ] );
    }
    free( array );
}


static
Assertions * assertion_new__no_ids( void )
{
    Assertion * const a = assertion_new( 1 + 1 == 2, 0 );
    assertion_assert_valid( *a );
    Assertions * const as = assertions(
        a->result == true,
        strcmp( a->expr, "1 + 1 == 2" ) == 0,
        assertion_ids_is_empty( *( a->ids ) )
    );
    assertion_free( a );
    return as;
}

#include <assert.h>

static
Assertions * assertion_new__some_ids( void )
{
    // Given:
    int const x = 3;
    #define ID_EXPRS x, 34, 2 + 3

    // When:
    Assertion * const a = assertion_new( 5 < 10, ID_EXPRS );

    // Then:
    assertion_assert_valid( *a );
    Assertions * const as = assertions(
        a->result == true,
        strcmp( a->expr, "5 < 10" ) == 0,
        assertion_ids_eq_each( *( a->ids ), ID_EXPRS )
    );
    assertion_free( a );
    #undef EXPR
    #undef EXPR_STR
    #undef ID_EXPRS
    return as;
}


static
Assertions * assertion_copy__gives_equal( void )
{
    Assertion * * const array = make_ex_assertions();
    Assertions * const as = assertions_empty();
    for ( size_t i = 0; array[ i ] != NULL; i += 1 ) {
        Assertion const orig = *( array[ i ] );
        Assertion * const copy = assertion_copy( orig );
        assertion_assert_valid( *copy );
        assertions_add( as, assertion_eq( *copy, orig ), i );
    }
    free_ex_assertions( array );
    return as;
}


static
Assertions * assertion_eq__works( void )
{
    Assertion * * const array = make_ex_assertions();
    Assertions * const as = assertions_empty();
    for ( size_t i = 0; array[ i ] != NULL; i += 1 ) {
        for ( size_t j = 0; array[ j ] != NULL; j += 1 ) {
            bool const eq_ij = assertion_eq( *( array[ i ] ),
                                             *( array[ j ] ) );
            bool const eq_ji = assertion_eq( *( array[ j ] ),
                                             *( array[ i ] ) );
            if ( i == j ) {
                assertions_add( as, eq_ij, i, j );
                assertions_add( as, eq_ji, i, j );
            } else {
                assertions_add( as, !eq_ij, i, j );
                assertions_add( as, !eq_ji, i, j );
            }
        }
    }
    free_ex_assertions( array );
    return as;
}


Test const assertion_tests[] = TEST_ARRAY(
    assertion_new__no_ids,
    assertion_new__some_ids,
    assertion_copy__gives_equal,
    assertion_eq__works
);


/* // ---------- */
/* // Example functions and data for use in testing. */
/* // ---------- */
/*  */
/* // Used as an example of a function call as a boolean expression. */
/* static bool numbers_eq( int x, int y, int z ) { return x == y && y == z; } */
/*  */
/* // Used as an example of a macro that evaluates to a boolean expression. */
/* #define NUMBERS_EQ_5( Y, Z ) numbers_eq( 5, Y, Z ) */
/*  */
/* // `TEST_REQUIRE` passes, and no further assertions are made. */
/* static TestAssertion * ex_require_pass_1( void ) { */
/*     for ( int i = 0; i < 10; i += 1 ) { */
/*         TEST_REQUIRE( i != 11, i ); */
/*     } */
/*     return NULL; */
/* } */
/*  */
/* // `TEST_REQUIRE` passes, and an assertion is made afterwards. */
/* static TestAssertion * ex_require_pass_2( void ) { */
/*     for ( int i = 0; i < 10; i += 1 ) { */
/*         TEST_REQUIRE( i != 11, i ); */
/*     } */
/*     return test_assert( 1 == 1 ); */
/* } */
/*  */
/* // `TEST_REQUIRE` fails with one identification expressions. */
/* static TestAssertion * ex_require_fail_1( void ) { */
/*     for ( int i = 0; i < 10; i += 1 ) { */
/*         TEST_REQUIRE( i != 7, i ); */
/*     } */
/*     // Should not get here. */
/*     return NULL; */
/* } */
/*  */
/* // `TEST_REQUIRE` fails with three identification expressions. */
/* static TestAssertion * ex_require_fail_2( void ) { */
/*     int xs[] = { 1, 2, 4, 3, 8 }; */
/*     for ( size_t i = 0; i < ( sizeof xs ) - 1; i += 1 ) { */
/*         TEST_REQUIRE( xs[ i ] < xs[ i + 1 ], i, xs[ i ], xs[ i + 1 ] ); */
/*     } */
/*     // Should not get here. */
/*     return test_assert( 1 + 1 == 2 ); */
/* } */
/*  */
/* // None of these assertions should be equal. */
/* static Assertion const assertions[] = { */
/*  */
/*     // No identifications. */
/*     { .expr = "",           .result = false, .ids = assertion_ids_= 0, .ids = NULL }, */
/*     { .expr = "1 + 1 == 2", .result = false, .num_ids = 0, .ids = NULL }, */
/*     { .expr = "blah",       .result = false, .num_ids = 0, .ids = NULL }, */
/*     { .expr = "",           .result = true,  .num_ids = 0, .ids = NULL }, */
/*     { .expr = "1 + 1 == 2", .result = true,  .num_ids = 0, .ids = NULL }, */
/*     { .expr = "blah",       .result = true,  .num_ids = 0, .ids = NULL }, */
/*  */
/*     // One identification expression. */
/*     { .expr = "",           .result = false, .num_ids = 1, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } }, */
/*     { .expr = "1 + 1 == 2", .result = false, .num_ids = 1, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } }, */
/*     { .expr = "blah",       .result = false, .num_ids = 1, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } }, */
/*     { .expr = "",           .result = true,  .num_ids = 1, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } }, */
/*     { .expr = "1 + 1 == 2", .result = true,  .num_ids = 1, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } }, */
/*     { .expr = "blah",       .result = true,  .num_ids = 1, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } }, */
/*  */
/*     // Two identification expressions. */
/*     { .expr = "",           .result = false, .num_ids = 2, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 }, */
/*                                     { .expr = "xs[ i ]", .value = -2 } } }, */
/*     { .expr = "1 + 1 == 2", .result = false, .num_ids = 2, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 }, */
/*                                     { .expr = "xs[ i ]", .value = -2 } } }, */
/*     { .expr = "blah",       .result = false, .num_ids = 2, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 }, */
/*                                     { .expr = "xs[ i ]", .value = -2 } } }, */
/*     { .expr = "",           .result = true,  .num_ids = 2, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 }, */
/*                                     { .expr = "xs[ i ]", .value = -2 } } }, */
/*     { .expr = "1 + 1 == 2", .result = true,  .num_ids = 2, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 }, */
/*                                     { .expr = "xs[ i ]", .value = -2 } } }, */
/*     { .expr = "blah",       .result = true,  .num_ids = 2, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 }, */
/*                                     { .expr = "xs[ i ]", .value = -2 } } }, */
/*     { .expr = "blah",       .result = true,  .num_ids = 2, */
/*       .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 }, */
/*                                     { .expr = "ys[ i ]", .value = -2 } } } */
/*  */
/* }; */
/*  */
/* // ---------- */
/* // End of example functions and data. */
/* // ---------- */
/*  */
/*  */
/* static */
/* TestAssertion * test_assertion_eq__works( void ) */
/* { */
/*     TestAssertion * const as = test_assertions_empty(); */
/*     for ( size_t i = 0; i < NELEM( ex_as ); i += 1 ) { */
/*         for ( size_t j = 0; j < NELEM( ex_as ); j += 1 ) { */
/*             bool const eq_ij = test_assertion_eq( ex_as[ i ], ex_as[ j ] ); */
/*             bool const eq_ji = test_assertion_eq( ex_as[ j ], ex_as[ i ] ); */
/*             if ( j == i ) { */
/*                 test_assertions_add( as, eq_ij, i, j ); */
/*                 test_assertions_add( as, eq_ji, i, j ); */
/*             } else { */
/*                 test_assertions_add( as, !eq_ij, i, j ); */
/*                 test_assertions_add( as, !eq_ji, i, j ); */
/*             } */
/*         } */
/*     } */
/*     return as; */
/* } */
/*  */
/*  */
/* static */
/* TestAssertion * test_assertion_eq__equal( void ) */
/* { */
/*     // These assertions should be equal, because the identification */
/*     // expressions are only compared for `num_ids` elements. */
/*     TestAssertion const equal[] = { */
/*         { .expr = "1 + 1 == 2", .result = true,  .num_ids = 0, .ids = NULL }, */
/*         { .expr = "1 + 1 == 2", .result = true,  .num_ids = 0, */
/*           .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 } } }, */
/*         { .expr = "1 + 1 == 2", .result = true,  .num_ids = 0, */
/*           .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 5 }, */
/*                                         { .expr = "xs[ i ]", .value = -2 } } } */
/*     }; */
/*     TestAssertion * const as = test_assertions_empty(); */
/*     for ( size_t i = 0; i < NELEM( as ); i += 1 ) { */
/*         for ( size_t j = 0; j < NELEM( as ); j += 1 ) { */
/*             test_assertions_add( as, */
/*                 test_assertion_eq( equal[ i ], equal[ j ] ), i, j ); */
/*             test_assertions_add( as, */
/*                 test_assertion_eq( equal[ j ], equal[ i ] ), i, j ); */
/*         } */
/*     } */
/*     return as; */
/* } */
/*  */
/*  */
/* static */
/* TestAssertion * test_assertion_is_end__correct( void ) */
/* { */
/*     TestAssertion * const as = test_assert( */
/*         test_assertion_is_end( ex_as[ 0 ] ) */
/*     ); */
/*     for ( size_t i = 1; i < NELEM( ex_as ); i += 1 ) { */
/*         test_assertions_add( as, !test_assertion_is_end( ex_as[ i ] ), i ); */
/*     } */
/*     return as; */
/* } */
/*  */
/*  */
/* static */
/* TestAssertion * test_assert__gives_right_assertions( void ) */
/* { */
/*     TestAssertion * const new = test_assert( */
/*         4 / 2 == 3, */
/*         numbers_eq( 12, 3 + 9, 24 / 2 ), */
/*         NUMBERS_EQ_5( 2 + 3, 7 - 3 ) == false */
/*     ); */
/*     TestAssertion * const as = test_assert( */
/*         test_assertion_eq( new[ 0 ], ( TestAssertion ){ */
/*             .expr = "4 / 2 == 3", .result = false */
/*         } ), */
/*         test_assertion_eq( new[ 1 ], ( TestAssertion ){ */
/*             .expr = "numbers_eq( 12, 3 + 9, 24 / 2 )", .result = true */
/*         } ), */
/*         // Macros are expanded before being stringified. */
/*         test_assertion_eq( new[ 2 ], ( TestAssertion ){ */
/*             .expr = "numbers_eq( 5, 2 + 3, 7 - 3 ) == 0", .result = true */
/*         } ), */
/*         test_assertion_eq( new[ 3 ], ( TestAssertion ){ .expr = NULL } ) */
/*     ); */
/*     test_assertions_free( new ); */
/*     return as; */
/* } */
/*  */
/*  */
/* static */
/* TestAssertion * TEST_REQUIRE__returns_only_when_false( void ) */
/* { */
/*     TestAssertion * const pass1 = ex_require_pass_1(); // == NULL */
/*     TestAssertion * const pass2 = ex_require_pass_2(); */
/*     TestAssertion * const fail1 = ex_require_fail_1(); */
/*     TestAssertion * const fail2 = ex_require_fail_2(); */
/*     TestAssertion * const as = test_assert( */
/*         // `ex_require_pass_1()` makes no assertions after its */
/*         // `TEST_REQUIRE` loop. */
/*         pass1 == NULL, */
/*         // `ex_require_pass_2()` makes a single assertion with a */
/*         // single expression after its `TEST_REQUIRE` loop. */
/*         test_assertion_eq( pass2[ 0 ], ( TestAssertion ){ */
/*             .expr = "1 == 1", .result = true */
/*         } ), */
/*         test_assertion_is_end( pass2[ 1 ] ), */
/*         // `ex_require_fails_1()` fails at a call to `TEST_REQUIRE` */
/*         // with a single identification expression. */
/*         test_assertion_eq( fail1[ 0 ], ( TestAssertion ){ */
/*             .expr = "i != 7", .result = false, .num_ids = 1, */
/*             .ids = ( TestAssertionId[] ){ { .expr = "i", .value = 7 } } */
/*         } ), */
/*         test_assertion_is_end( fail1[ 1 ] ), */
/*         // `ex_require_fails_2()` fails at a call to `TEST_REQUIRE` */
/*         // with three identification expressions. */
/*         test_assertion_eq( fail2[ 0 ], ( TestAssertion ){ */
/*             .expr = "xs[ i ] < xs[ i + 1 ]", .result = false, .num_ids = 3, */
/*             .ids = ( TestAssertionId[] ){ */
/*                 { .expr = "i", .value = 2 }, */
/*                 { .expr = "xs[ i ]", .value = 4 }, */
/*                 { .expr = "xs[ i + 1 ]", .value = 3 } */
/*             } */
/*         } ), */
/*         test_assertion_is_end( fail2[ 1 ] ) */
/*     ); */
/*     test_assertions_free( pass2 ); */
/*     test_assertions_free( fail1 ); */
/*     test_assertions_free( fail2 ); */
/*     return as; */
/* } */
/*  */
/*  */
/* Test const assertion_tests[] = TESTS( */
/*     test_assertion_eq__nonequal, */
/*     test_assertion_eq__equal, */
/*     test_assertion_is_end__correct, */
/*     test_assert__gives_right_assertions, */
/*     TEST_REQUIRE__returns_only_when_false */
/* ); */
/*  */
