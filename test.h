// tests/test.h

#ifndef INCLUDED_TEST_H
#define INCLUDED_TEST_H


#include <stdlib.h>     // NULL
#include <stdbool.h>    // bool


typedef struct Test {

    bool ( *func )( void );
    // Returns true if the test passes, and false if it fails.

    char const * name;
    // Used for printing the results of running this test.

} Test;


typedef struct TestAssertion {
    char const * expr;
    bool result;
} TestAssertion;


typedef struct TestResults {
    int passed;
    int failed;
} TestResults;

typedef struct TestFail {
    char * condition;
}


#define TEST( f ) { .name = #f, .func = f }


#define TEST_END { .name = NULL, .func = NULL }


#define TEST_ASSERT1( e ) \
    if ( !( e ) ) { \
        return #e;
    }


TestResults test_run_all( char const * name, Test const * tests );


int test__return_val( TestResults const * results );


#define test_return_val( ... ) \
    test__return_val( ( TestResults[] ){ __VA_ARGS__, { .failed = -1 } } )


#endif // INCLUDED_TEST_H
