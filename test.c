// test.c

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


#include "test.h" // Test

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "assertion.h" // TestAssertion, test_assertion*
#include "_common.h" // string_eq


bool test_eq( Test const t1, Test const t2 )
{
    return t1.func == t2.func
        && string_eq( t1.name, t2.name );
}


static
char * repeat( char const * const string, size_t const times )
{
    char * const new = calloc( ( strlen( string ) * times ) + 1, 1 );
    for ( size_t i = 0; i < times; i += 1 ) {
        strcat( new, string );
    }
    return new;
}


bool test_run_( struct test_run_options const o )
{
    Test const test = o.test;
    FILE * const file = ( o.file == NULL ) ? stdout : o.file;
    char const * const indent = ( o.indent == NULL ) ? "" : o.indent;

    Assertions * const as = test.func();
    assert( as != NULL );
    bool const passed = assertions_all_true( *as );
    fprintf( file, "%s%s:  %s\n",
             indent, passed ? "pass" : "fail", test.name );
    if ( !passed ) {
        char * const indent2 = repeat( indent, 2 );
        char * const indent3 = repeat( indent, 3 );
        assertions_print( false, .assertions = *as,
                                 .file = file,
                                 .assertion_indent = indent2,
                                 .ids_indent = indent3 );
        free( indent2 );
        free( indent3 );
    }
    assertions_free( as );
    return passed;
}


int tests_run_( struct tests_run_options const o )
{
    char const * const name = o.name;
    assert( name != NULL );
    Test const * const tests = o.tests;
    assert( tests != NULL );
    FILE * const file = ( o.file == NULL ) ? stdout : o.file;
    char const * const indent = ( o.indent == NULL ) ? "  " : o.indent;

    fprintf( file, "Running %s tests...\n", name );
    int failed = 0;
    for ( size_t i = 0; tests[ i ].func != NULL; i += 1 ) {
        bool const passed = test_run( .test = tests[ i ],
                                      .file = file,
                                      .indent = indent );
        if ( !passed ) {
            failed += 1;
        }
    }
    return failed;
}


int tests_return_val_( int const * const fails )
{
    for ( size_t i = 0; fails[ i ] != -1; i += 1 ) {
        if ( fails[ i ] != 0 ) {
            return 1;
        }
    }
    return 0;
}

