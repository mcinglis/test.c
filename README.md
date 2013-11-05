# Test.c

Test.c is a testing library for C that encourages modular and composable tests.

``` c
#include "test.h"

struct TestAssertion * addition_works( void )
{
    return test_assert( 2 + 2 == 4,
                        ( 1 + 1 != 2 ) || ( 9 - 3 == 6 ) );
}

struct TestAssertion * multiplication_works( void )
{
    return test_assert( 1 * 5 == 1,
                        9 * 2 == 18,
                        3 * 4 != 12 );
}

struct TestAssertion * some_numbers_dont_exist( void )
{
    for ( int x = 0; x < 100; x += 1 ) {
        TEST_REQUIRE( x != 17 && x != 42, x );
    }
    return NULL;
}

struct Test const arithmetic_tests[] = TESTS( addition_works,
                                              multiplication_works,
                                              some_numbers_dont_exist );

int main( void ) {
    tests_run( "arithmetic", arithmetic_tests )
    // Running arithmetic tests...
    //     pass: addition_works
    //     fail: multiplication_works
    //         false: 1 * 5 == 1
    //         false: 3 * 4 != 12
    //     fail: some_numbers_dont_exist
    //         false: x != 17 && x != 42    (x = 17)
    // Finished arithmetic tests: 1 passed, and 2 failed.
}
```

[`test.h`](/test.h) has the complete interface and documentation.

Files that `#include <test.h>` also need to be able to `#include <macromap.h/macromap.h>`, from [Macromap.h](https://github.com/mcinglis/macromap.h). Have a look at [Trie.c](https://github.com/mcinglis/trie.c) for an example of how to manage this (and for an extensive example of using Test.c).


## Building

Test.c compiles with GCC 4.8 and Clang 3.3. It should compile with any C11 compiler that can [generate dependency files](/Makefile#L24).

``` sh
# To run the default development build (with debugging symbols):
$ make
# To build with optimizations and without debugging symbols:
$ CFLAGS='-std=c11 -O2' make
# To use your non-default C compiler:
$ CC=clang make
# If you don't have a C11 compiler, it can compile under C99 (for now):
$ CFLAGS='-std=c99' make
```


## License

Copyright (C) 2013 Malcolm Inglis <http://minglis.id.au/>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but **without any warranty**; without even the implied warranty of **merchantability** or **fitness for a particular purpose**. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

Contact me if you'd like to use this program under different terms.

