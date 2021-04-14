#include <gympl/testing.hpp>
#include <type_traits>

//#define INDEX_BASE_DISABLE_DEFINE_LITERALS
//#define INDEX_BASE_DISABLE_DEFINE_USINGS
#include <inba/inba.hpp>

namespace
{
    void basic_runtime_tests()
    {
        using namespace inba;

        auto fr = []() -> Indexed_i0 { return Indexed_i0{1}; };
        Indexed_i0 const l(2);
        auto fl = [&l]() -> Indexed_i0 const& { return l; };
        auto fx = []() -> Indexed_i0 { return std::move(Indexed_i0{3}); };

        auto f1r = []() -> Indexed_i1 { return Indexed_i1{1}; };
        Indexed_i1 const l1(2);
        auto f1l = [&l1]() -> Indexed_i1 const& { return l1; };
        auto f1x = []() -> Indexed_i1 { return std::move(Indexed_i1{3}); };

        Indexed_i0 i0cr(fr());
        GYMPL_REQUIRE(static_cast<int>(i0cr) == 1);
        Indexed_i0 i0cl(fl());
        GYMPL_REQUIRE(static_cast<int>(i0cl) == 2);
        Indexed_i0 i0cx(fx());
        GYMPL_REQUIRE(static_cast<int>(i0cx) == 3);

        Indexed_i0 i0c1r(f1r());
        GYMPL_REQUIRE(static_cast<int>(i0c1r) == 0);
        Indexed_i0 i0c1l(f1l());
        GYMPL_REQUIRE(static_cast<int>(i0c1l) == 1);
        Indexed_i0 i0c1x(f1x());
        GYMPL_REQUIRE(static_cast<int>(i0c1x) == 2);

        Indexed_i0 i0ar;
        i0ar = fr();
        GYMPL_REQUIRE(static_cast<int>(i0ar) == 1);
        Indexed_i0 i0al;
        i0al = fl();
        GYMPL_REQUIRE(static_cast<int>(i0al) == 2);
        Indexed_i0 i0ax;
        i0ax = fx();
        GYMPL_REQUIRE(static_cast<int>(i0ax) == 3);

        Indexed_i0 i0a1r;
        i0a1r = f1r();
        GYMPL_REQUIRE(static_cast<int>(i0a1r) == 0);
        Indexed_i0 i0a1l;
        i0a1l = f1l();
        GYMPL_REQUIRE(static_cast<int>(i0a1l) == 1);
        Indexed_i0 i0a1x;
        i0a1x = f1x();
        GYMPL_REQUIRE(static_cast<int>(i0a1x) == 2);

        Indexed_i0 i0a0i = 5_i0 + 1;
        GYMPL_REQUIRE(static_cast<int>(i0a0i) == 6);
        Indexed_i0 i0ai0 = 5 + 2_i0;
        GYMPL_REQUIRE(static_cast<int>(i0ai0) == 7);
        Indexed_i0 i0a00 = 5_i0 + 3_i0;
        GYMPL_REQUIRE(static_cast<int>(i0a00) == 8);

        Indexed_i0 i0s0i = 5_i0 - 1;
        GYMPL_REQUIRE(static_cast<int>(i0s0i) == 4);
        Indexed_i0 i0si0 = 5 - 2_i0;
        GYMPL_REQUIRE(static_cast<int>(i0si0) == 3);
        int i0s00 = 5_i0 - 3_i0;
        GYMPL_REQUIRE(i0s00 == 2);

        Indexed_i0 i0m0i = 5_i0 * 2;
        GYMPL_REQUIRE(static_cast<int>(i0m0i) == 10);
        Indexed_i0 i0mi0 = 3 * 5_i0;
        GYMPL_REQUIRE(static_cast<int>(i0mi0) == 15);

        Indexed_i0 i0d0i = 6_i0 / 2;
        GYMPL_REQUIRE(static_cast<int>(i0d0i) == 3);
        Indexed_i0 i0di0 = 6 / 3_i0;
        GYMPL_REQUIRE(static_cast<int>(i0di0) == 2);

        Indexed_i0 i0oe = 10_i0;
        i0oe += 1_i0;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 11);
        i0oe += 2;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 13);
        i0oe -= 3_i0;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 10);
        i0oe -= 4;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 6);
        i0oe *= 4;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 24);
        i0oe /= 3;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 8);

        ++i0oe;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 9);
        i0oe++;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 10);
        --i0oe;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 9);
        i0oe--;
        GYMPL_REQUIRE(static_cast<int>(i0oe) == 8);

        GYMPL_REQUIRE(5_i0 == 5_i0);
        GYMPL_REQUIRE(5_i0 >= 5_i0);
        GYMPL_REQUIRE(5_i0 >= 4_i0);
        GYMPL_REQUIRE(5_i0 > 4_i0);
        GYMPL_REQUIRE(5_i0 <= 5_i0);
        GYMPL_REQUIRE(4_i0 <= 5_i0);
        GYMPL_REQUIRE(4_i0 < 5_i0);

        GYMPL_REQUIRE(5_i0 == 6_i1);
        GYMPL_REQUIRE(5_i0 >= 6_i1);
        GYMPL_REQUIRE(5_i0 >= 5_i1);
        GYMPL_REQUIRE(5_i0 > 5_i1);
        GYMPL_REQUIRE(5_i0 <= 6_i1);
        GYMPL_REQUIRE(4_i0 <= 6_i1);
        GYMPL_REQUIRE(4_i0 < 6_i1);

#ifdef GYMPL_HAS_SPACESHIP
        GYMPL_REQUIRE(5_i0 <=> 5_i0 == std::strong_ordering::equal);
        GYMPL_REQUIRE(5_i0 <=> 4_i0 == std::strong_ordering::greater);
        GYMPL_REQUIRE(4_i0 <=> 5_i0 == std::strong_ordering::less);

        GYMPL_REQUIRE(5_i0 <=> 6_i1 == std::strong_ordering::equal);
        GYMPL_REQUIRE(5_i0 <=> 5_i1 == std::strong_ordering::greater);
        GYMPL_REQUIRE(4_i0 <=> 6_i1 == std::strong_ordering::less);
#endif
    }
}

GYMPL_TEST_MAIN_START
{
    basic_runtime_tests();
    return EXIT_SUCCESS;
}
GYMPL_TEST_MAIN_END
