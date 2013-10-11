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


typedef struct TestResults {
    int passed;
    int failed;
} TestResults;


#define TEST( f ) ( ( Test const ){ .name = #f, .func = f } )


#define TEST_END ( ( Test const ){ .func = NULL, .name = NULL } )


TestResults test_run_all( char const * name, Test const * tests );


int test__return_val( TestResults const * results );


#define test_return_val( ... ) \
    test__return_val( ( TestResults[] ){ __VA_ARGS__, { .failed = -1 } } )


#endif // INCLUDED_TEST_H
