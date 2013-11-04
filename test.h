// tests/test.h

#ifndef INCLUDED_TEST_H
#define INCLUDED_TEST_H


#include <stdlib.h>
#include <stdbool.h>

#include <macromap.h/macromap.h>        // MACROMAP


// An assertion is a boolean expression and its evaluation. It may be
// identified by an integer, so as to provide more information than its
// expression (e.g. the expression may consist of variables, but readers
// can't know the values of those variables without a way to number each
// assertion).
typedef struct TestAssertion {
    char const * expr;
    bool result;
    bool has_id;
    int id;
} TestAssertion;


// A test is a named function that returns an array of assertions, which
// may be true or false.
typedef struct Test {
    // Returns an array of assertions terminated by an assertion with
    // a null `expr` field.
    TestAssertion * ( *func )( void );
    // Used for printing the results of running this test.
    char const * name;
} Test;


typedef struct TestResults {
    int passed;
    int failed;
} TestResults;

typedef struct TestFail {
    char * condition;
}


#define TEST( f ) { .name = #f, .func = f }


#define TEST_END { .name = NULL, .func = NULL }

// Counts how many assertions there are in the given array up to and
// including the terminating assertion with a null expression field.
size_t test_assert_size( TestAssertion const * const assertions )

// Copies an array of assertions, terminated by an assertion with a null
// expression field, onto the heap.
TestAssertion * test_assert_alloc_( TestAssertion const * assertions );

// Saves users of `test_assert_alloc` from having to type the typecast
// or terminate the array with an element with a null expression.
#define test_assert_alloc( ... ) \
    test_assert_alloc_( ( TestAssertion[] ){ \
        __VA_ARGS__, { .expr = NULL } \
    } );

// Creates an element of an assertion list by stringifying the given
// expression. The trailing comma separates each element when building
// an array of assertions with `MACROMAP`.
#define TEST_ASSERT_EL( expr ) \
    ( TestAssertion ){ .expr = #expr, .result = ( expr ), .has_id = false },

// Takes a variable number of expressions, and returns a pointer to an
// array of assertions on the heap, with each assertion corresponding to
// a given expression.
#define test_assert( ... ) \
    test_assert_alloc( MACROMAP( TEST_ASSERT_EL, __VA_ARGS__ ) )

// If the given expression evaluates to false, then this causes the
// containing function to return a single-element assertion array
// corresponding to the given expression. The single assertion will
// be identified with the given `id` argument.
#define TEST_REQUIRE( expr, id ) { \
    bool result = ( expr ); \
    if ( !result ) { \
        return test_assert_alloc( \
            { .expr = #expr, .result = result, .has_id = true, .id = id } \
        ); \
    } \
}


TestResults test_run_all( char const * name, Test const * tests );


int test_return_val_( TestResults const * results );


#define test_return_val( ... ) \
    test_return_val_( ( TestResults[] ){ __VA_ARGS__, { .failed = -1 } } )


#endif // INCLUDED_TEST_H
