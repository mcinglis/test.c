# Test.c

Test.c is a nice testing library for C. It encourages your tests to be modular, reusable, and composable.

``` c
#include "test.h"

TestAssertion * addition_works( void )
{
    return test_assert(
        5 + 0 == 5,
        2 + 4 == 7
    );
}

TestAssertion * multiplication_works( void )
{
    return test_assert(
        3 * 4 == 12,
        9 * 2 == 18
    );
}

TestAssertion * some_numbers_dont_exist( void )
{
    for ( int x = 0; x < 100; x += 1 ) {
        TEST_REQUIRE( x != 17 && x != 42, x );
    }
    return NULL;
}

Test const arithmetic_tests[] = TESTS(
    addition_works,
    multiplication_works,
    some_numbers_dont_exist
);

int main( void ) {
    test_run_all( "arithmetic", arithmetic_tests )
    // Running arithmetic tests...
    //     fail: addition_works
    //         false: 2 + 4 == 7
    //     pass: multiplication_works
    //     fail: some_numbers_dont_exist
    //         false: x != 17 && x != 42    (id = 17)
    // Finished arithmetic tests: 1 passed, and 2 failed.
}
```

See [`test.h`](/test.h) for the complete interface and documentation.


## Building

Test.c should compile with any C11 compiler, such as GCC 4.8 or Clang 3.3.

``` sh
# To run the default development build (with debugging symbols):
$ make
# To build with optimizations and without debugging symbols:
$ CFLAGS='-std=c11 -O2' make
# To use your non-default C compiler:
$ CC=clang make
# If you don't have a C11 compiler, it can compile under C99 (for now):
$ CFLAGS=-std=c99 make
```


## License

Copyright (C) 2013 Malcolm Inglis <me@minglis.id.au>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but **without any warranty**; without even the implied warranty of **merchantability** or **fitness for a particular purpose**. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

Contact me if you'd like to use this program under different terms.

