#pragma once
#ifndef INCLUDED_INBA_TESTS_BI
#define INCLUDED_INBA_TESTS_BI

#include <type_traits>
//#define INDEX_BASE_DISABLE_DEFINE_LITERALS
//#define INDEX_BASE_DISABLE_DEFINE_USINGS
#include <inba/inba.hpp>

namespace inba
{
    namespace tests
    {

        //! [base value class]
        struct Bi
        {
            using value_type = int;
            using diff_type = int;

            value_type i_;

            constexpr Bi()
              : i_()
            {}
            // CWG2327 workaround http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2327
            template<typename T, typename std::enable_if<inba::cv_ref_removed_is_same<T, value_type>::value, bool>::type = true>
            constexpr Bi(T v)
              : i_(v)
            {}

            // CWG2327 workaround http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2327
            constexpr operator value_type() const noexcept
            {
                return i_;
            }

            constexpr Bi const& operator+() const noexcept
            {
                return *this;
            }
            constexpr Bi operator+(Bi const& o) const noexcept(GYMPL_NOEXCEPT_OP(Bi(i_ + o.i_)))
            {
                return Bi(i_ + o.i_);
            }

            GYMPL_CONSTEXPR_11 Bi& operator+=(Bi const& o) noexcept(GYMPL_NOEXCEPT_OP(i_ += o.i_))
            {
                i_ += o.i_;
                return *this;
            }
            GYMPL_CONSTEXPR_11 Bi& operator+=(value_type v) noexcept(GYMPL_NOEXCEPT_OP(i_ += v))
            {
                i_ += v;
                return *this;
            }

            constexpr Bi operator-() const noexcept(GYMPL_NOEXCEPT_OP(Bi(-i_)))
            {
                return Bi(-i_);
            }
            constexpr diff_type operator-(Bi const& o) const noexcept(GYMPL_NOEXCEPT_OP(diff_type(i_ - o.i_)))
            {
                return i_ - o.i_;
            }
            GYMPL_CONSTEXPR_11 Bi& operator-=(Bi const& o) noexcept(GYMPL_NOEXCEPT_OP(i_ -= o.i_))
            {
                i_ -= o.i_;
                return *this;
            }
            GYMPL_CONSTEXPR_11 Bi& operator-=(value_type v) noexcept(GYMPL_NOEXCEPT_OP(i_ -= v))
            {
                i_ -= v;
                return *this;
            }
            constexpr Bi operator*(Bi const& o) const noexcept(GYMPL_NOEXCEPT_OP(Bi(i_* o.i_)))
            {
                return Bi(i_ * o.i_);
            }
            GYMPL_CONSTEXPR_11 Bi& operator*=(value_type i) noexcept(GYMPL_NOEXCEPT_OP(i_ *= i))
            {
                i_ *= i;
                return *this;
            }
            GYMPL_CONSTEXPR_11 Bi& operator*=(Bi const& o) noexcept(GYMPL_NOEXCEPT_OP(i_ *= o.i_))
            {
                i_ *= o.i_;
                return *this;
            }
            constexpr Bi operator/(Bi const& o) const noexcept(GYMPL_NOEXCEPT_OP(Bi(i_ / o.i_)))
            {
                return Bi(i_ / o.i_);
            }
            GYMPL_CONSTEXPR_11 Bi& operator/=(value_type i) noexcept(GYMPL_NOEXCEPT_OP(i_ /= i))
            {
                i_ /= i;
                return *this;
            }
            GYMPL_CONSTEXPR_11 Bi& operator/=(Bi const& o) noexcept(GYMPL_NOEXCEPT_OP(i_ /= o.i_))
            {
                i_ /= o.i_;
                return *this;
            }

            GYMPL_CONSTEXPR_11 Bi& operator++() & noexcept(GYMPL_NOEXCEPT_OP(++i_))
            {
                ++i_;
                return *this;
            }
            GYMPL_CONSTEXPR_11 Bi operator++(int) & noexcept(GYMPL_NOEXCEPT_OP(Bi(++(*this))))
            {
                Bi result(*this);
                ++(*this);
                return result;
            }
            GYMPL_CONSTEXPR_11 Bi& operator--() & noexcept(GYMPL_NOEXCEPT_OP(--i_))
            {
                --i_;
                return *this;
            }
            GYMPL_CONSTEXPR_11 Bi operator--(int) & noexcept(GYMPL_NOEXCEPT_OP(Bi(--(*this))))
            {
                Bi result(*this);
                --(*this);
                return result;
            }
        };

        inline constexpr Bi operator+(Bi const& left, Bi::value_type right) noexcept(GYMPL_NOEXCEPT_OP(Bi(left.i_ + right)))
        {
            return Bi(left.i_ + right);
        }
        inline constexpr Bi operator+(Bi::value_type const& left, Bi right) noexcept(GYMPL_NOEXCEPT_OP(Bi(left + right.i_)))
        {
            return Bi(left + right.i_);
        }
        inline constexpr Bi operator-(Bi const& left, Bi::value_type right) noexcept(GYMPL_NOEXCEPT_OP(Bi(left.i_ - right)))
        {
            return Bi(left.i_ - right);
        }
        inline constexpr Bi operator*(Bi const& left, Bi::value_type right) noexcept(GYMPL_NOEXCEPT_OP(Bi(left.i_* right)))
        {
            return Bi(left.i_ * right);
        }
        inline constexpr Bi operator*(Bi::value_type const& left, Bi right) noexcept(GYMPL_NOEXCEPT_OP(Bi(left* right.i_)))
        {
            return Bi(left * right.i_);
        }
        inline constexpr Bi operator/(Bi const& left, Bi::value_type right) noexcept(GYMPL_NOEXCEPT_OP(Bi(left.i_ / right)))
        {
            return Bi(left.i_ / right);
        }
        inline constexpr Bi operator/(Bi::value_type const& left, Bi right) noexcept(GYMPL_NOEXCEPT_OP(Bi(left / right.i_)))
        {
            return Bi(left / right.i_);
        }
        //! [base value class]

#ifdef GYMPL_HAS_CONCEPTS
        static_assert(inba::concepts::StronglyInDecrementable<Bi>);
        static_assert(inba::concepts::CanDoArithmetics<Bi>);
        static_assert(inba::concepts::IsArrayIndex<Bi>);
        static_assert(inba::concepts::Addable<Bi>);
        static_assert(inba::concepts::Subtractable<Bi>);
        static_assert(inba::concepts::Multiplyable<Bi>);
        static_assert(inba::concepts::Dividable<Bi>);
#endif

        inline Bi operator"" _Bi(unsigned long long int i)
        {
            return Bi{(int)i};
        }

#ifdef GYMPL_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
        //! [user defined literals c++20]
        inline inba::Indexed<Bi, Bi{0}> operator"" _bi0(unsigned long long int i)
        {
            return inba::Indexed<Bi, Bi{0}>(Bi{(int)i});
        }
        inline inba::Indexed<Bi, Bi{1}> operator"" _bi1(unsigned long long int i)
        {
            return inba::Indexed<Bi, Bi{1}>(Bi{(int)i});
        }
        //! [user defined literals c++20]

        //! [typedefs c++20]
        using Indexed_Bi0 = inba::Indexed<Bi, Bi{0}>;
        using Indexed_Bi1 = inba::Indexed<Bi, Bi{1}>;
        //! [typedefs c++20]
#else
        //! [user defined literals pre c++20]
        inline inba::Indexed<Bi, inba::cnttp::CreateIndexedHelper<Bi, int, 0>> operator"" _bi0(unsigned long long int i)
        {
            return inba::Indexed<Bi, inba::cnttp::CreateIndexedHelper<Bi, int, 0>>(Bi{(int)i});
        }
        inline inba::Indexed<Bi, inba::cnttp::CreateIndexedHelper<Bi, int, 1>> operator"" _bi1(unsigned long long int i)
        {
            return inba::Indexed<Bi, inba::cnttp::CreateIndexedHelper<Bi, int, 1>>(Bi{(int)i});
        }
        //! [user defined literals pre c++20]

        //! [typedefs pre c++20]
        using Indexed_Bi0 = inba::Indexed<Bi, inba::cnttp::CreateIndexedHelper<Bi, int, 0>>;
        using Indexed_Bi1 = inba::Indexed<Bi, inba::cnttp::CreateIndexedHelper<Bi, int, 1>>;
        //! [typedefs pre c++20]
#endif

        static_assert(inba::is_IndexedBase_compatible<Indexed_Bi0>::value GYMPL_NTSA);

    }
}

#endif
