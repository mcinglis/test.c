// tests/test.c

#include <stdbool.h>    // bool
#include <stdio.h>      // NULL, printf

#include "test.h" // Test


static
TestResults test_run( Test const test,
                      TestResults results )
{
    if ( test.func() == true ) {
        printf( "  pass: %s\n", test.name );
        results.passed += 1;
    } else {
        printf( "  fail: %s\n", test.name );
        results.failed += 1;
    }
    return results;
}


TestResults test_run_all( char const * const name,
                          Test const * const tests )
{
    printf( "Running %s tests...\n", name );
    TestResults results = { .passed = 0, .failed = 0 };
    for ( int i = 0; tests[ i ].func != NULL; i += 1 ) {
        results = test_run( tests[ i ], results );
    }
    printf( "Finished %s tests: %d passed, and %d failed.\n",
            name, results.passed, results.failed );
    return results;
}


int test__return_val( TestResults const * const results )
{
    for ( int i = 0; results[ i ].failed != -1; i += 1 ) {
        if ( results[ i ].failed > 0 ) {
            return 1;
        }
    }
    return 0;
}

