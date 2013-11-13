# Test.c

[![Travis CI build status](https://api.travis-ci.org/mcinglis/test.c.png)](https://travis-ci.org/mcinglis/test.c)

Test.c is a modern, simple testing library for C.

``` c
#include <stdlib.h>
#include <test.c/test.h>

void * before_each( void ) {
    int const xs[] = { 2, 4, 8, 5, 3 };
    int * const mem = malloc( sizeof( xs ) );
    memcpy( mem, xs, sizeof( xs ) );
    return mem;
}

void after_each( void * const data ) {
    free( data );
}

struct TestAssertion * addition_works( void * const data ) {
    return test_assert( 3 + 8 == 11 );
}

struct TestAssertion * multiplication_works( void * const data ) {
    int * const xs = data;
    xs[ 2 ] = 3;
    return test_assert( 2 * 2 == 5,
                        3 * xs[ 2 ] == 9,
                        xs[ 1 ] * 4 != 16 );
}

struct TestAssertion * xs_is_increasing( void * const data ) {
    int const * const xs = data;
    for ( int i = 0; i < 4; i += 1 ) {
        TEST_REQUIRE( xs[ i ] <= xs[ i + 1 ], i, xs[ i ], xs[ i + 1 ] );
    }
    return NULL;
}

struct Test const number_tests[] = TESTS_FIX( before_each, after_each,
    addition_works,
    multiplication_works,
    xs_is_increasing
);

int main( void ) {
    tests_run( "number", number_tests );
    // Running number tests...
    //     pass:  addition_works
    //     fail:  multiplication_works
    //         false:  2 * 2 == 5
    //         false:  xs[ 1 ] * 4 != 16
    //     fail:  xs_is_increasing
    //         false:  xs[ i ] <= xs[ i + 1 ]
    //             (for i = 2, xs[ i ] = 8, xs[ i + 1 ] = 5)
    // Finished number tests: 1 passed, and 2 failed.
}
```

The `Test` and `TestAssertion` structs are typedef'd with the same name, so using `struct` with them is optional. I usually leave it off.

[`test.h`](/test.h) and [`assertion.h`](/assertion.h) have the complete public interface and documentation. There are [`examples/`](/examples/) which are compiled with `make`. Test.c's [`tests/`](/tests/) are written with Test.c. You can read those for an extensive example of using Test.c, and to see its particular behaviors.

Files that include `test.h` or `assertion.h` need to be able to `#include <macromap.h/macromap.h>`, from [Macromap.h](https://github.com/mcinglis/macromap.h). [`Module.mk`](/Module.mk) is provided to make this easier.


## Releases

I try to [tag](http://git-scm.com/book/en/Git-Basics-Tagging) the releases according to [semantic versioning v2.0.0](http://semver.org/spec/v2.0.0.html); the public interface is `test.h` and `assertion.h`. You can browse the releases [on GitHub](https://github.com/mcinglis/test.c/releases), but because this project uses [submodules](http://git-scm.com/book/en/Git-Tools-Submodules), the archives generated by GitHub won't compile. You have to clone this repository if you want to build it.


## Building

Test.c compiles with GCC 4.8 and Clang 3.2 (and later). It should compile with any C11 compiler that can [generate dependency files](/Makefile#L77).

``` sh
# Initialize and update the submodules (you need to do this once after
# cloning, or pulling a commit which updates a submodule):
$ git submodule update --init --recursive

# Then, to run the default development build (with debugging symbols):
$ make
# To build with optimizations and without debugging symbols:
$ make CFLAGS='-std=c11 -O2'
# To use your non-default C compiler:
$ make CC=clang
# If you don't have a C11 compiler, it can compile under C99 (for now):
$ make CFLAGS='-std=c99'
```


## Tests

```
$ make test
Running assertion tests...
    pass:  test_assertion_eq__is_correct_for_neq
    pass:  test_assertion_eq__is_correct_for_eq
    pass:  test_assertion_is_end__is_correct
    pass:  test_assert__gives_right_assertions
    pass:  TEST_REQUIRE__returns_only_when_false
Finished assertion tests: 5 passed, and 0 failed.
Running test tests...
    pass:  test_eq__is_correct
    pass:  test_is_end__is_correct
    pass:  TESTS__gives_right_tests
    pass:  TESTS_FIX__gives_right_tests
    pass:  tests_run__gives_right_results
    pass:  tests_return_val__gives_0_when_all_pass
    pass:  tests_return_val__gives_1_when_some_fail
Finished test tests: 7 passed, and 0 failed.
```


## Projects using Test.c

- Test.c
- [Trie.c](https://github.com/mcinglis/trie.c)

Please let me know if you know of any others. It would be great to hear that other people find this useful.


## License

Copyright (C) 2013 Malcolm Inglis <http://minglis.id.au/>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but **without any warranty**; without even the implied warranty of **merchantability** or **fitness for a particular purpose**. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

Contact me if you'd like to use this program under different terms.

