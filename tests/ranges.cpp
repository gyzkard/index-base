#include "testing.hpp"

//#define INDEX_BASE_DISABLE_DEFINE_LITERALS
//#define INDEX_BASE_DISABLE_DEFINE_USINGS
#include <inba/inba.hpp>

#ifdef INBA_HAS_RANGES

#include "bi.hpp"

#include <algorithm>
#include <array>
#include <ranges>
#include <type_traits>

namespace
{
    void basic_runtime_tests()
    {
        using namespace inba;
        using namespace inba::tests;

        //! [built in types]
        std::array<Indexed_i0, 4> ai0;
        std::array<Indexed_i1, 4> ai1;
        std::ranges::generate(ai0, [n = 0]() mutable { return Indexed_i0(n++); });
        std::ranges::generate(ai1, [n = 1]() mutable { return Indexed_i1(n++); });
        INBA_REQUIRE(std::ranges::equal(ai0, ai1));
        //! [built in types]
    }
}

// see https://stackoverflow.com/questions/66944119/how-can-i-tell-the-compiler-that-mycustomtype-is-equality-comparable-with-someot
//! [base value class adapter]
namespace
{
    template<typename T>
    class Bi_EqProxy
    {
        template<typename>
        friend class Bi_EqProxy;

    private:
        T ref_;

    public:
        template<typename U>
        requires std::convertible_to<U, T> constexpr Bi_EqProxy(U ref)
          : ref_(ref)
        {}

        template<typename U>
        requires std::equality_comparable_with<T, U> constexpr bool operator==(const Bi_EqProxy<U>& rhs) const
        {
            return ref_ == rhs.ref_;
        };
    };
}
namespace std
{
    using inba::tests::Bi;

    template<template<typename> class LQ, template<typename> class RQ>
    struct basic_common_reference<inba::Indexed<Bi, Bi{0}>, inba::Indexed<Bi, Bi{1}>, LQ, RQ>
    {
        using type = Bi_EqProxy<Bi>;
    };
    template<template<typename> class LQ, template<typename> class RQ>
    struct basic_common_reference<inba::Indexed<Bi, Bi{1}>, inba::Indexed<Bi, Bi{0}>, LQ, RQ>
    {
        using type = Bi_EqProxy<Bi>;
    };
}
//! [base value class adapter]

namespace
{
    void extended_runtime_tests()
    {
        using namespace inba;
        using namespace inba::tests;

        //! [base value class]
        std::array<Indexed_Bi0, 4> ai0;
        std::array<Indexed_Bi1, 4> ai1;
        std::ranges::generate(ai0, [n = 0]() mutable { return Indexed_Bi0(n++); });
        std::ranges::generate(ai1, [n = 1]() mutable { return Indexed_Bi1(n++); });
        INBA_REQUIRE(std::ranges::equal(ai0, ai1));
        //! [base value class]
    }
}
#endif

namespace
{
    void runtime_tests()
    {
#ifdef INBA_HAS_RANGES
        basic_runtime_tests();
        extended_runtime_tests();
#endif
    }
}

TEST_MAIN_START
{
    runtime_tests();
    return EXIT_SUCCESS;
}
TEST_MAIN_END
