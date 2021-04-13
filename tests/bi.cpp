#include "bi.hpp"

#include "testing.hpp"

#include <type_traits>

//#define INDEX_BASE_DISABLE_DEFINE_LITERALS
//#define INDEX_BASE_DISABLE_DEFINE_USINGS
#include <inba/inba.hpp>

namespace
{
    void extended_runtime_tests()
    {
        using namespace inba;
        using namespace inba::tests;

        auto fr = []() -> Indexed_Bi0 { return Indexed_Bi0{1}; };
        Indexed_Bi0 const l(2);
        auto fl = [&l]() -> Indexed_Bi0 const& { return l; };
        auto fx = []() -> Indexed_Bi0 { return std::move(Indexed_Bi0{3}); };

        auto f1r = []() -> Indexed_Bi1 { return Indexed_Bi1{1}; };
        Indexed_Bi1 const l1(2);
        auto f1l = [&l1]() -> Indexed_Bi1 const& { return l1; };
        auto f1x = []() -> Indexed_Bi1 { return std::move(Indexed_Bi1{3}); };

        Indexed_Bi0 i0cr(fr());
        INBA_REQUIRE(static_cast<int>(i0cr) == 1);
        Indexed_Bi0 i0cl(fl());
        INBA_REQUIRE(static_cast<int>(i0cl) == 2);
        Indexed_Bi0 i0cx(fx());
        INBA_REQUIRE(static_cast<int>(i0cx) == 3);

        Indexed_Bi0 i0c1r(f1r());
        INBA_REQUIRE(static_cast<int>(i0c1r) == 0);
        Indexed_Bi0 i0c1l(f1l());
        INBA_REQUIRE(static_cast<int>(i0c1l) == 1);
        Indexed_Bi0 i0c1x(f1x());
        INBA_REQUIRE(static_cast<int>(i0c1x) == 2);

        Indexed_Bi0 i0ar;
        i0ar = fr();
        INBA_REQUIRE(static_cast<int>(i0ar) == 1);
        Indexed_Bi0 i0al;
        i0al = fl();
        INBA_REQUIRE(static_cast<int>(i0al) == 2);
        Indexed_Bi0 i0ax;
        i0ax = fx();
        INBA_REQUIRE(static_cast<int>(i0ax) == 3);

        Indexed_Bi0 i0a1r;
        i0a1r = f1r();
        INBA_REQUIRE(static_cast<int>(i0a1r) == 0);
        Indexed_Bi0 i0a1l;
        i0a1l = f1l();
        INBA_REQUIRE(static_cast<int>(i0a1l) == 1);
        Indexed_Bi0 i0a1x;
        i0a1x = f1x();
        INBA_REQUIRE(static_cast<int>(i0a1x) == 2);

        5_bi0 + 1_Bi;
        Indexed_Bi0 i0a0i = 5_bi0 + 1_Bi;
        INBA_REQUIRE(static_cast<int>(i0a0i) == 6);
        Indexed_Bi0 i0ai0 = 5_Bi + 2_bi0;
        INBA_REQUIRE(static_cast<int>(i0ai0) == 7);
        Indexed_Bi0 i0a00 = 5_bi0 + 3_bi0;
        INBA_REQUIRE(static_cast<int>(i0a00) == 8);

        Indexed_Bi0 i0s0i = 5_bi0 - 1_Bi;
        INBA_REQUIRE(static_cast<int>(i0s0i) == 4);
        Indexed_Bi0 i0si0 = 5_Bi - 2_bi0;
        INBA_REQUIRE(static_cast<int>(i0si0) == 3);
        int i0s00 = 5_bi0 - 3_bi0;
        INBA_REQUIRE(i0s00 == 2);

        Indexed_Bi0 i0m0i = 5_bi0 * 2_Bi;
        INBA_REQUIRE(static_cast<int>(i0m0i) == 10);
        Indexed_Bi0 i0mi0 = 3_Bi * 5_bi0;
        INBA_REQUIRE(static_cast<int>(i0mi0) == 15);

        Indexed_Bi0 i0d0i = 6_bi0 / 2_Bi;
        INBA_REQUIRE(static_cast<int>(i0d0i) == 3);
        Indexed_Bi0 i0di0 = 6_Bi / 3_bi0;
        INBA_REQUIRE(static_cast<int>(i0di0) == 2);

        Indexed_Bi0 i0oe = 10_bi0;
        i0oe += 1_bi0;
        INBA_REQUIRE(static_cast<int>(i0oe) == 11);
        i0oe += 2;
        INBA_REQUIRE(static_cast<int>(i0oe) == 13);
        i0oe -= 3_bi0;
        INBA_REQUIRE(static_cast<int>(i0oe) == 10);
        i0oe -= 4;
        INBA_REQUIRE(static_cast<int>(i0oe) == 6);
        i0oe *= 4;
        INBA_REQUIRE(static_cast<int>(i0oe) == 24);
        i0oe /= 3;
        INBA_REQUIRE(static_cast<int>(i0oe) == 8);

        ++i0oe;
        INBA_REQUIRE(static_cast<int>(i0oe) == 9);
        i0oe++;
        INBA_REQUIRE(static_cast<int>(i0oe) == 10);
        --i0oe;
        INBA_REQUIRE(static_cast<int>(i0oe) == 9);
        i0oe--;
        INBA_REQUIRE(static_cast<int>(i0oe) == 8);

        INBA_REQUIRE(5_bi0 == 5_bi0);
        INBA_REQUIRE(5_bi0 >= 5_bi0);
        INBA_REQUIRE(5_bi0 >= 4_bi0);
        INBA_REQUIRE(5_bi0 > 4_bi0);
        INBA_REQUIRE(5_bi0 <= 5_bi0);
        INBA_REQUIRE(4_bi0 <= 5_bi0);
        INBA_REQUIRE(4_bi0 < 5_bi0);

        INBA_REQUIRE(5_bi0 == 6_bi1);
        INBA_REQUIRE(5_bi0 >= 6_bi1);
        INBA_REQUIRE(5_bi0 >= 5_bi1);
        INBA_REQUIRE(5_bi0 > 5_bi1);
        INBA_REQUIRE(5_bi0 <= 6_bi1);
        INBA_REQUIRE(4_bi0 <= 6_bi1);
        INBA_REQUIRE(4_bi0 < 6_bi1);

#ifdef INBA_HAS_SPACESHIP
        INBA_REQUIRE(5_bi0 <=> 5_bi0 == std::strong_ordering::equal);
        INBA_REQUIRE(5_bi0 <=> 4_bi0 == std::strong_ordering::greater);
        INBA_REQUIRE(4_bi0 <=> 5_bi0 == std::strong_ordering::less);

        INBA_REQUIRE(5_bi0 <=> 6_bi1 == std::strong_ordering::equal);
        INBA_REQUIRE(5_bi0 <=> 5_bi1 == std::strong_ordering::greater);
        INBA_REQUIRE(4_bi0 <=> 6_bi1 == std::strong_ordering::less);
#endif
    }
}

TEST_MAIN_START
{
    extended_runtime_tests();
    return EXIT_SUCCESS;
}
TEST_MAIN_END
