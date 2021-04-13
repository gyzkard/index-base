#include "testing.hpp"

#ifdef INBA_PRINT_CONFIG

#pragma message(INBA_PRINTDEFINE(INBA_COMPILER))
#ifdef INBA_HAS_CPP20
#pragma message(INBA_TOSTR2(INBA_HAS_CPP20))
#endif
#ifdef INBA_HAS_CPP17
#pragma message(INBA_TOSTR2(INBA_HAS_CPP17))
#endif
#ifdef INBA_HAS_CPP14
#pragma message(INBA_TOSTR2(INBA_HAS_CPP14))
#endif

#ifdef INBA_HAS_RANGES
#pragma message(INBA_TOSTR2(INBA_HAS_RANGES))
#endif
#ifdef INBA_HAS_CONCEPTS
#pragma message(INBA_TOSTR2(INBA_HAS_CONCEPTS))
#endif
#ifdef INBA_HAS_SPACESHIP
#pragma message(INBA_TOSTR2(INBA_HAS_SPACESHIP))
#endif
#ifdef INBA_HAS_REMOVE_CV_REF
#pragma message(INBA_TOSTR2(INBA_HAS_REMOVE_CV_REF))
#endif
#ifdef INBA_HAS_NOEXCEPT_OP
#pragma message(INBA_TOSTR2(INBA_HAS_NOEXCEPT_OP))
#endif

#endif
