# Test.c

Test.c is a nice testing library for C. It encourages your tests to be reusable, composable and modular, without sacrificing convenience or usability.

``` c
bool new_alphabet_should_be_valid( void ) { ... }

Test const alphabet_tests[] = {
    TEST( new_alphabet_should_be_valid ),
    TEST_END
};

bool new_trie_should_be_valid( void ) { ... }
bool trie_should_only_have_added_strings( void ) { ... }

Test const trie_tests[] = {
    {
        .name = "this is a different name for the test",
        .func = trie_should_only_have_added_strings
    },
    TEST( new_trie_should_be_valid ),
    TEST_END
};

int main( void ) {
    return test_return_val(
        test_run_all( alphabet_tests ),
        test_run_all( trie_tests )
    );
}
```

Ironically, there are no tests yet, so use this at your own risk.


## Building

Test.c should compile with any C11 compiler, such as GCC 4.8 or Clang 3.3.

``` sh
# To run the default development build (with debugging symbols):
$ make
# To make it go a bit faster:
$ make CFLAGS='-std=c11 -O2'
# To use your non-default C compiler:
$ make CC=clang
# If you don't have a C11 compiler, it can compile under C99 (for now):
$ make CFLAGS=-std=c99
```


## License

Test.c is provided under the [GNU General Public License](/LICENSE.md).

