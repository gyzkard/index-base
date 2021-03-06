#include <gympl/testing.hpp>
#include <type_traits>

//#define INDEX_BASE_DISABLE_DEFINE_LITERALS
//#define INDEX_BASE_DISABLE_DEFINE_USINGS
#include <inba/inba.hpp>

namespace
{
    namespace
    {
#ifndef GYMPL_HAS_CPP17
        template<typename R, typename T>
        constexpr R getAs(T t)
        {
            return static_cast<R>(t);
        }
#endif
    }
    void basic_compiletime_tests()
    {
        using namespace inba;

        static_assert(Indexed_i0(0).value() == 0 GYMPL_NTSA);
        static_assert(Indexed_i0(0).value() != 1 GYMPL_NTSA);
        static_assert(Indexed_i0(0).value() >= 0 GYMPL_NTSA);
        static_assert(Indexed_i0(0).value() <= 0 GYMPL_NTSA);
        static_assert(Indexed_i0(0).value() > -1 GYMPL_NTSA);
        static_assert(Indexed_i0(0).value() < 1 GYMPL_NTSA);
        static_assert(0 == Indexed_i0(0).value() GYMPL_NTSA);
        static_assert(1 != Indexed_i0(0).value() GYMPL_NTSA);
        static_assert(0 >= Indexed_i0(0).value() GYMPL_NTSA);
        static_assert(0 <= Indexed_i0(0).value() GYMPL_NTSA);
        static_assert(-1 < Indexed_i0(0).value() GYMPL_NTSA);
        static_assert(1 > Indexed_i0(0).value() GYMPL_NTSA);

        static_assert(Indexed_uz0().value() == 0 GYMPL_NTSA);
        static_assert(Indexed_uz1().value() == 1 GYMPL_NTSA);
        static_assert(Indexed_uz0{}.value() == 0 GYMPL_NTSA);
        static_assert(Indexed_uz1{}.value() == 1 GYMPL_NTSA);
        static_assert(Indexed_uz0{0}.value() == 0 GYMPL_NTSA);
        static_assert(Indexed_uz1{1}.value() == 1 GYMPL_NTSA);

        static_assert(Indexed_uci0{0}.value() == 0 GYMPL_NTSA);
        static_assert(Indexed_uci0(256).value() == 0 GYMPL_NTSA);

#ifdef GYMPL_HAS_CPP17
        static_assert([]() {
            Indexed_uci0 v = Indexed_uci0{Indexed_uci1{3}};
            return v == 2;
        }() GYMPL_NTSA);
        static_assert([]() {
            Indexed_uci0 v;
            v = Indexed_uci1{3};
            return v.value() == 2;
        }() GYMPL_NTSA);
        static_assert([](int i) { return i; }((int)Indexed_uci1{3}) == 2 GYMPL_NTSA);
        static_assert([](char i) { return i; }((int)Indexed_i1{3}) == 2 GYMPL_NTSA);
#else
        static_assert(Indexed_uci0(Indexed_uci1{3}).value() == 2 GYMPL_NTSA);
        {
            constexpr Indexed_uci0 v = Indexed_uci0(Indexed_uci1{3});
            static_assert(v == static_cast<unsigned char>(2) GYMPL_NTSA);
        }
        static_assert(getAs<int>(Indexed_uci1{3}) == 2 GYMPL_NTSA);
        static_assert(getAs<int>(Indexed_i1{3}) == 2 GYMPL_NTSA);
#endif
        static_assert(static_cast<Indexed_uci0>(Indexed_uci1{3}).value() == 2 GYMPL_NTSA);

        static_assert(Indexed_uz0{} == Indexed_uz0 {} GYMPL_NTSA);
        static_assert(Indexed_uz0{} == Indexed_uz1 {} GYMPL_NTSA);
        static_assert(Indexed_uz0{} == 0 GYMPL_NTSA);

        constexpr char s[] = "Test";
        constexpr Indexed_uz0 index0{2};
        constexpr Indexed_uz1 index1{2};
        static_assert(index0.value() == 2 GYMPL_NTSA);
        static_assert(index1.value() == 2 GYMPL_NTSA);
        static_assert(static_cast<size_t>(index0) == 2 GYMPL_NTSA);
        static_assert(static_cast<size_t>(index1) == 1 GYMPL_NTSA);
        static_assert(s[(int)index0] == 's' GYMPL_NTSA);
        static_assert(s[(int)index1] == 'e' GYMPL_NTSA);
    }
}

GYMPL_TEST_MAIN_START
{
    return EXIT_SUCCESS;
}
GYMPL_TEST_MAIN_END
