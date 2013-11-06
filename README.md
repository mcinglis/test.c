# Test.c

Test.c is a modern, simple testing library for C.

``` c
#include "test.h"

void * before_each( void )
{
    int * const xs = malloc( sizeof( *xs ) * 3 );
    xs[ 0 ] = 1;
    xs[ 1 ] = 2;
    xs[ 2 ] = 4;
    return xs;
}

void after_each( void * const data )
{
    int * const xs = data;
    free( xs );
}

struct TestAssertion * addition_works( void * const data )
{
    int * const xs = data;
    xs[ 1 ] = 3;
    return test_assert( xs[ 1 ] + 8 == 11 );
}

struct TestAssertion * multiplication_works( void * const data )
{
    int const * const xs = data;
    return test_assert( 2 * 2 == 5,
                        3 * xs[ 1 ] == 6,
                        xs[ 2 ] * 4 != 16 );
}

struct TestAssertion * some_numbers_dont_exist( void * const data )
{
    for ( int n = 0; n < 100; n += 1 ) {
        TEST_REQUIRE( n != 17 && n != 42, n );
    }
    return NULL;
}

struct Test const number_tests[] = TESTS_FIX( before_each, after_each,
    addition_works,
    multiplication_works,
    some_numbers_dont_exist
);

int main( void ) {
    tests_run( "number", number_tests );
    // Running number tests...
    //     pass:  addition_works
    //     fail:  multiplication_works
    //         false:  2 * 2 == 5
    //         false:  xs[ 2 ] * 4 != 16
    //     fail:  some_numbers_dont_exist
    //         false for n = 17:  n != 17 && n != 42
    // Finished number tests: 0 passed, and 2 failed.
}
```

The `Test` and `TestAssertion` structs are `typedef`'d with the same name, so using `struct` with them is optional. It's used above as a demonstration, but I usually leave it off.

[`test.h`](/test.h) has the complete interface and documentation.

Files that include `test.h` also need to be able to `#include <macromap.h/macromap.h>`, from [Macromap.h](https://github.com/mcinglis/macromap.h). Have a look at [Trie.c](https://github.com/mcinglis/trie.c) for an example of how to manage this (and for an extensive example of using Test.c).


## Releases

Releases are [tagged](http://git-scm.com/book/en/Git-Basics-Tagging) with [semantic versioning v2.0.0](http://semver.org/spec/v2.0.0.html). You can also browse the releases [on GitHub](https://github.com/mcinglis/test.c/releases), but because this project uses [Git submodules](http://git-scm.com/book/en/Git-Tools-Submodules) the archive files generated by GitHub won't compile. You have to clone this repository if you want to build it.


## Building

Test.c compiles with GCC 4.8 and Clang 3.3. It should compile with any C11 compiler that can [generate dependency files](/Makefile#L24).

``` sh
# After cloning the repository, or pulling a commit that updates a submodule:
$ make submodules

# Then, to run the default development build (with debugging symbols):
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

