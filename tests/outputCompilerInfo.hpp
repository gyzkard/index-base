#include "testing.hpp"

#ifdef _MSC_VER
#pragma message(INBA_PRINTDEFINE(__cplusplus))
#pragma message(INBA_PRINTDEFINE(_MSC_FULL_VER))
#else
#pragma message(INBA_PRINTDEFINE(__cplusplus))
#if defined(__clang_major__)
#pragma message("clang version: " INBA_TOSTR(__clang_major__) "." INBA_TOSTR(__clang_minor__) "." INBA_TOSTR(__clang_patchlevel__))
#endif
#if defined(__GNUC__)
#pragma message("gcc version: " INBA_TOSTR(__GNUC__) "." INBA_TOSTR(__GNUC_MINOR__))
#endif
#endif
