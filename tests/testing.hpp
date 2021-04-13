#pragma once
#ifndef INCLUDED_INBA_TESTING
#define INCLUDED_INBA_TESTING

#include <cassert>

#define INBA_REQUIRE(expr) (void)((! ! (expr)) || (assert(expr), std::exit(EXIT_FAILURE), 0))

#define INBA_TOSTR2(x) #x
#define INBA_TOSTR(x) INBA_TOSTR2(x)

#define INBA_PRINTDEFINE(x) #x ": " INBA_TOSTR2(x)

#ifndef INBA_NON_SELF_CONTAINED_TESTS
#define TEST_MAIN_START int main()
#else
#define TEST_MAIN_START \
    namespace           \
    {                   \
        int r = []()
#endif

#ifdef INBA_NON_SELF_CONTAINED_TESTS
#define TEST_MAIN_END \
    ();               \
    }
#else
#define TEST_MAIN_END
#endif

#endif
