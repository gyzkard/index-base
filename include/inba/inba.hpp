/// \file inba.hpp

#pragma once
#ifndef INCLUDED_INBA_INBA
// \cond macro-verbose-255
#define INCLUDED_INBA_INBA

#if defined(_MSC_VER)
#define INBA_COMPILER "MSVC"
#define INBA_COMPILER_MSVC
// todo: will ms ever consider
#define _Pragma(s)
#elif defined(__clang_major__)
#define INBA_COMPILER "CLANG"
#define INBA_COMPILER_CLANG
#elif defined(__GNUC__)
#define INBA_COMPILER "GCC"
#define INBA_COMPILER_GCC
#endif

#if __cplusplus >= 201703L
#define INBA_HAS_CPP17
#endif
// todo: msvc  defines __cplusplus as 201703L and /std:c++latest is not yet totally c++20 (and all this only with /Zc:__cplusplus
// enabled),
//       clang defines __cplusplus as 202002L and -std=c++20 also isn't totally c++20 yet,
//       gcc   defines __cplusplus as 201705L with -std=c++20,
//       so:
//#if __cplusplus >= 202002L
#if __cplusplus >= 201705L
#define INBA_HAS_CPP20

#if defined(INBA_COMPILER_CLANG) && (__clang_major__ > 13 || (__clang_major__ == 13 && __clang_minor__ >= 0)) || \
    defined(INBA_COMPILER_GCC) && (__GNUC__ > 10 || (__GNUC__ == 10 && __GNUC_MINOR__ >= 2)) ||                  \
    ! defined(INBA_COMPILER_CLANG) && ! defined(INBA_COMPILER_GCC)
// todo: Who knows when clang will have these available.
//       Adapt the check accordingly when it will be clear.
//       Atm (with Clang 11.0.1) , it seems like the trunk has them and it shows version 13.0.0, at least on godbolt.
#define INBA_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
#define INBA_HAS_RANGES
#endif

#if defined(INBA_COMPILER_CLANG) && (__clang_major__ > 11 || (__clang_major__ == 11 && __clang_patchlevel__ >= 1)) || \
    defined(INBA_COMPILER_GCC) && (__GNUC__ > 10 || (__GNUC__ == 10 && __GNUC_MINOR__ >= 2)) ||                       \
    ! defined(INBA_COMPILER_CLANG) && ! defined(INBA_COMPILER_GCC)
#define INBA_HAS_CONCEPTS
#define INBA_HAS_SPACESHIP
#define INBA_HAS_REMOVE_CV_REF
#endif

#endif
#if ! defined(INBA_COMPILER_GCC) || __GNUC__ > 10 || (__GNUC__ == 10 && __GNUC_MINOR__ >= 2)
#define INBA_HAS_NOEXCEPT_OP
#define INBA_NOEXCEPT_OP(...) noexcept(__VA_ARGS__)
#else
#define INBA_NOEXCEPT_OP(...) false
#endif
#if ! defined(INBA_COMPILER_MSVC) && (! defined(INBA_COMPILER_GCC) || __GNUC__ > 10 || (__GNUC__ == 10 && __GNUC_MINOR__ >= 2))
#define INBA_HAS_VALID_SFINAE_NOEXCEPT_OP(...) noexcept(__VA_ARGS__)
#else
#define INBA_HAS_VALID_SFINAE_NOEXCEPT_OP(...) false
#endif
#if __cplusplus >= 201402L
#define INBA_HAS_CPP14
#endif

#if defined(INBA_HAS_CPP20) &&                                                                                         \
    (defined(INBA_COMPILER_CLANG) && (__clang_major__ > 11 || (__clang_major__ == 11 && __clang_patchlevel__ >= 1)) || \
     defined(INBA_COMPILER_GCC) && (__GNUC__ > 10 || (__GNUC__ == 10 && __GNUC_MINOR__ >= 2)) ||                       \
     ! defined(INBA_COMPILER_CLANG) && ! defined(INBA_COMPILER_GCC))
#define INBA_HAS_is_nothrow_convertible
#endif

#ifdef INBA_HAS_CPP17
#define INBA_HAS_DEFINITIONLESS_STATIC_CONSTEXPR_DATA_MEMBER
#endif

#include <algorithm>
#include <type_traits>
#ifdef INBA_HAS_SPACESHIP
#include <compare>
#endif

#ifdef INBA_HAS_CONCEPTS
#define INBA_COT(mp_c) mp_c
#define INBA_COREQUIRES(...) requires(__VA_ARGS__)
#define INBA_PRE_CONCEPTS_CHECK(...)
#else
#define INBA_COT(mp_c) typename
#define INBA_COREQUIRES(...)
#define INBA_PRE_CONCEPTS_CHECK(...) static_assert(__VA_ARGS__)
#define INBA_CPP11
#endif

#ifdef INBA_HAS_CPP17
#define INBA_NTSA
#else
#define INBA_NTSA , "todo"
#endif

#ifdef INBA_HAS_CPP14
#define INBA_CONSTEXPR_11 constexpr
#else
#define INBA_CONSTEXPR_11
#endif
// \endcond

#if _MSC_VER <= 1900
#pragma warning(push)
#pragma warning(disable : 4814) // MS
#endif

namespace inba
{
    namespace compatibility
    {
#ifndef INBA_HAS_CPP17
        template<bool B>
        using bool_constant = std::integral_constant<bool, B>;
#else
        using std::bool_constant;
#endif

#ifdef INBA_HAS_REMOVE_CV_REF
        using std::remove_cvref;
#else
        template<class T>
        struct remove_cvref
        {
            typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
        };
#endif
    }
    namespace mpl
    {
        using namespace compatibility;

        template<typename T, typename... args>
        struct First
        {
            typedef T type;
        };

        struct NotMyType
        {};
        template<typename T, int I>
        struct bool_to_type
        {
            typedef NotMyType type;
        };
        template<typename T>
        struct bool_to_type<T, 1>
        {
            typedef T type;
        };

        template<typename T, int I>
        struct bool_to_types_type
        {
            typedef NotMyType type;
        };
        template<typename T>
        struct bool_to_types_type<T, 1>
        {
            typedef typename T::type type;
        };

        struct Match
        {};

        template<typename T, typename U, typename H = Match>
        struct is_same_to_type
        {};
        template<typename T, typename U>
        struct is_same_to_type<T, U, typename bool_to_type<Match, std::is_same<T, U>::value>::type>
        {
            typedef void type;
        };

        template<typename T>
        decltype(std::declval<typename std::add_lvalue_reference<T>::type>()) Instantiate()
        {
            return std::declval<typename std::add_lvalue_reference<T>::type>();
        }

        template<template<typename, typename> class OP, typename T, typename U = T>
        struct op2x : bool_constant<OP<T, U>::value && OP<U, T>::value>
        {};
        template<template<typename, typename> class OP, typename T, typename U = T>
        struct op2 : bool_constant<OP<T, T>::value && OP<U, U>::value && op2x<OP, T, U>::value>
        {};
    }

    namespace traits
    {
        using namespace compatibility;
        using namespace mpl;

        using std::false_type;
        using std::true_type;

        template<class, class = void>
        struct has_type_member : false_type
        {};
#ifdef INBA_HAS_CPP17
        template<class T>
        struct has_type_member<T, std::void_t<typename T::type>> : true_type
        {};
        template<class T>
        inline constexpr bool has_type_member_v = has_type_member<T>::value;
#else
        template<class T>
        struct has_type_member<T, typename mpl::First<void, typename T::type>::type> : true_type
        {};
#endif

        template<typename T, typename U>
        struct cv_ref_removed_is_same : bool_constant<std::is_same<typename remove_cvref<T>::type, typename remove_cvref<U>::type>::value>
        {};

#ifdef INBA_HAS_CPP17
        template<typename T, typename U>
        inline constexpr bool cv_ref_removed_is_same_v = cv_ref_removed_is_same<T, U>::value;
#endif

        // c/p from ms type_traits..
        // \cond ns-detail
        namespace detail
        {
            template<typename From>
            struct copy_cv
            {
                template<typename To>
                using Apply = To;
            };
            template<typename From>
            struct copy_cv<const From>
            {
                template<typename To>
                using Apply = const To;
            };
            template<typename From>
            struct copy_cv<volatile From>
            {
                template<typename To>
                using Apply = volatile To;
            };
            template<typename From>
            struct copy_cv<const volatile From>
            {
                template<typename To>
                using Apply = const volatile To;
            };
        }
        // \endcond
        template<typename From, typename To>
        using copy_cv = typename detail::copy_cv<From>::template Apply<To>;

        template<typename From>
        struct copy_ref
        {
            template<class To>
            using Apply = copy_cv<From, To>;
        };
        template<typename From>
        struct copy_ref<From&>
        {
            template<class To>
            using Apply = typename std::add_lvalue_reference<copy_cv<From, To>>::type;
        };
        template<typename From>
        struct copy_ref<From&&>
        {
            template<class To>
            using Apply = typename std::add_rvalue_reference<copy_cv<From, To>>::type;
        };

        // \cond ns-detail
        namespace detail
        {
            template<typename T, typename Default, int IsIntegral>
            struct get_as_signed_type
            {
                typedef Default type;
            };
            template<typename T, typename Default>
            struct get_as_signed_type<T, Default, 1>
            {
                typedef typename copy_ref<T>::template Apply<typename std::make_signed<typename std::remove_reference<T>::type>::type>
                    type;
            };
        }
        // \endcond
        /// <summary>
        /// std::make_signed with fallback
        /// </summary>
        /// <param name="T">potentially unsigned value with a potentially signed representation</param>
        /// <param name="Default">Fallback type in case there's no signed representation of T</param>
        template<typename T, typename Default = T>
        struct get_as_signed_type
        {
            typedef typename detail::get_as_signed_type<T, Default, std::is_integral<typename remove_cvref<T>::type>::value>::type type;
        };

        // \cond impl
        namespace ops
        {
            using namespace mpl;

            template<typename T, typename U, typename H = Match>
            struct has_plus : false_type
            {};
            template<typename T, typename U>
            struct has_plus<T, U, typename First<Match, decltype(Instantiate<T>() + Instantiate<U>())>::type> : true_type
            {};

            template<typename T, typename U, typename H = Match>
            struct has_minus : false_type
            {};
            template<typename T, typename U>
            struct has_minus<T, U, typename First<Match, decltype(Instantiate<T>() - Instantiate<U>())>::type> : true_type
            {};

            template<typename T, typename U, typename H = Match>
            struct has_multiply : false_type
            {};
            template<typename T, typename U>
            struct has_multiply<T, U, typename First<Match, decltype(Instantiate<T>() * Instantiate<U>())>::type> : true_type
            {};

            template<typename T, typename U, typename H = Match>
            struct has_divide : false_type
            {};
            template<typename T, typename U>
            struct has_divide<T, U, typename First<Match, decltype(Instantiate<T>() / Instantiate<U>())>::type> : true_type
            {};

            template<typename T, typename H = Match>
            struct has_increment_prefix : false_type
            {};
            template<typename T>
            struct has_increment_prefix<T, typename First<Match, decltype(++Instantiate<T>())>::type> : true_type
            {};

            template<typename T, typename H = Match>
            struct has_increment_postfix : false_type
            {};
            template<typename T>
            struct has_increment_postfix<T, typename First<Match, decltype(Instantiate<T>()++)>::type> : true_type
            {};

            template<typename T, typename H = Match>
            struct is_weakly_incrementable : false_type
            {};
            template<typename T>
            struct is_weakly_incrementable<T, typename First<Match, decltype(++Instantiate<T>()++)>::type> : true_type
            {};

            template<typename T, typename H = Match>
            struct has_decrement_prefix : false_type
            {};
            template<typename T>
            struct has_decrement_prefix<T, typename First<Match, decltype(--Instantiate<T>())>::type> : true_type
            {};

            template<typename T, typename H = Match>
            struct has_decrement_postfix : false_type
            {};
            template<typename T>
            struct has_decrement_postfix<T, typename First<Match, decltype(Instantiate<T>()--)>::type> : true_type
            {};

            template<typename T, typename H = Match>
            struct is_weakly_decrementable : false_type
            {};
            template<typename T>
            struct is_weakly_decrementable<T, typename First<Match, decltype(--Instantiate<T>()--)>::type> : true_type
            {};
        }
        // \endcond
        template<typename T, typename U = T>
        struct has_plus : bool_constant<ops::has_plus<T, U>::value>
        {};
        template<typename T, typename U = T>
        struct has_minus : bool_constant<ops::has_minus<T, U>::value>
        {};
        template<typename T, typename U = T>
        struct has_multiply : bool_constant<ops::has_multiply<T, U>::value>
        {};
        template<typename T, typename U = T>
        struct has_divide : bool_constant<ops::has_divide<T, U>::value>
        {};
        template<typename T>
        struct has_increment_prefix : ops::has_increment_prefix<T>
        {};
        template<typename T>
        struct has_increment_postfix : ops::has_increment_postfix<T>
        {};
        template<typename T>
        struct has_decrement_prefix : ops::has_decrement_prefix<T>
        {};
        template<typename T>
        struct has_decrement_postfix : ops::has_decrement_postfix<T>
        {};
        template<typename T>
        struct is_weakly_incrementable : ops::is_weakly_incrementable<T>
        {};
        template<typename T>
        struct is_weakly_decrementable : ops::is_weakly_decrementable<T>
        {};

        template<typename T, typename U = T>
        struct is_addable : public mpl::op2<has_plus, T, U>
        {};
        template<typename T, typename U = T>
        struct is_subtractable : public mpl::op2<has_minus, T, U>
        {};
        template<typename T, typename U = T>
        struct is_multiplyable : public mpl::op2<has_multiply, T, U>
        {};
        template<typename T, typename U = T>
        struct is_dividable : public mpl::op2<has_divide, T, U>
        {};
        template<typename T, typename U = T>
        struct is_add_and_subtractable : bool_constant<is_addable<T, U>::value && is_subtractable<T, U>::value>
        {};
        template<typename T, typename U = T>
        struct is_multiply_and_dividable : bool_constant<is_multiplyable<T, U>::value && is_dividable<T, U>::value>
        {};

        template<typename T, typename U = T>
        struct is_addable_x : public mpl::op2x<has_plus, T, U>
        {};
        template<typename T, typename U = T>
        struct is_subtractable_x : public mpl::op2x<has_minus, T, U>
        {};
        template<typename T, typename U = T>
        struct is_multiplyable_x : public mpl::op2x<has_multiply, T, U>
        {};
        template<typename T, typename U = T>
        struct is_dividable_x : public mpl::op2x<has_divide, T, U>
        {};
        template<typename T, typename U = T>
        struct is_add_and_subtractable_x : bool_constant<is_addable_x<T, U>::value && is_subtractable_x<T, U>::value>
        {};
        template<typename T, typename U = T>
        struct is_multiply_and_dividable_x : bool_constant<is_multiplyable_x<T, U>::value && is_dividable_x<T, U>::value>
        {};

        template<typename T, typename U = T>
        struct can_do_arithmetics_x : bool_constant<is_add_and_subtractable_x<T, U>::value && is_multiply_and_dividable_x<T, U>::value>
        {};
        template<typename T, typename U = T>
        struct can_do_arithmetics : bool_constant<is_add_and_subtractable<T, U>::value && is_multiply_and_dividable<T, U>::value>
        {};
        template<typename T>
        struct is_strongly_incrementable
          : bool_constant<has_increment_prefix<T>::value && has_increment_postfix<T>::value && ! is_weakly_incrementable<T>::value>
        {};
        template<typename T>
        struct is_strongly_decrementable
          : bool_constant<has_decrement_prefix<T>::value && has_decrement_postfix<T>::value && ! is_weakly_decrementable<T>::value>
        {};
        template<typename T>
        struct is_strongly_in_and_decrementable
          : bool_constant<is_strongly_incrementable<T>::value && is_strongly_decrementable<T>::value>
        {};

        template<typename T, typename U>
        struct diff_type_unchecked
        {
            typedef decltype(Instantiate<T>() - Instantiate<U>()) type;
        };
        template<typename T, typename U>
        struct diff_type
        {
            typedef typename bool_to_types_type<diff_type_unchecked<T, U>, has_minus<T, U>::value>::type type;
        };

        template<typename T>
        struct is_array_index
          : bool_constant<(std::is_convertible<T, char>::value || std::is_convertible<T, unsigned char>::value ||
                           std::is_convertible<T, short>::value || std::is_convertible<T, unsigned short>::value ||
                           std::is_convertible<T, int>::value || std::is_convertible<T, unsigned int>::value ||
                           std::is_convertible<T, long>::value || std::is_convertible<T, unsigned long>::value ||
                           std::is_convertible<T, long long>::value || std::is_convertible<T, unsigned long long>::value ||
                           std::is_convertible<T, char>::value || std::is_convertible<T, unsigned char>::value) &&
                          ! (std::is_same<float, typename remove_cvref<T>::type>::value ||
                             std::is_same<double, typename remove_cvref<T>::type>::value)>
        {};

        template<typename T, typename B = T>
        struct is_indexed_value_type
          : bool_constant<is_strongly_in_and_decrementable<T>::value && is_add_and_subtractable_x<T, B>::value &&
                          is_array_index<T>::value>
        {};
        template<typename T, typename B = T>
        struct is_cv_ref_removed_indexed_value_type
          : bool_constant<is_indexed_value_type<typename remove_cvref<T>::type, typename remove_cvref<B>::type>::value>
        {};

        template<typename T, typename B>
        struct is_indexed_value_base_type : bool_constant<is_add_and_subtractable_x<T, B>::value>
        {};

        template<typename T, typename H = Match>
        struct has_Indexed_values : false_type
        {};
        template<typename T>
        struct has_Indexed_values<T, typename First<Match, decltype(Instantiate<T>().value(), Instantiate<T>().index_base())>::type>
          : true_type
        {};

        template<typename T, typename Default, typename H = Match>
        struct get_Indexed_value_return_type
        {
            typedef Default type;
        };
        template<typename T, typename Default>
        struct get_Indexed_value_return_type<T, Default, typename First<Match, decltype(Instantiate<T>().value())>::type>
        {
            typedef decltype(Instantiate<T>().value()) type;
        };
        template<typename T, typename Default>
        struct get_Indexed_value_type
        {
            typedef typename remove_cvref<typename get_Indexed_value_return_type<T, Default>::type>::type type;
        };

        template<typename T, typename Default, typename H = Match>
        struct get_Indexed_value_base_return_type
        {
            typedef Default type;
        };
        template<typename T, typename Default>
        struct get_Indexed_value_base_return_type<T, Default, typename First<Match, decltype(Instantiate<T>().index_base())>::type>
        {
            typedef typename remove_cvref<decltype(Instantiate<T>().index_base())>::type type;
        };
        template<typename T, typename Default>
        struct get_Indexed_value_base_type
        {
            typedef typename remove_cvref<typename get_Indexed_value_base_return_type<T, Default>::type>::type type;
        };

        template<typename T>
        struct is_pure_Indexed_assignable_value_type
          : bool_constant<! has_Indexed_values<T>::value && is_cv_ref_removed_indexed_value_type<T>::value>
        {};
        template<typename T>
        struct is_IndexedBase_compatible
          : bool_constant<is_indexed_value_type<typename get_Indexed_value_type<T, mpl::NotMyType>::type,
                                                typename get_Indexed_value_base_type<T, mpl::NotMyType>::type>::value>
        {};
    }

#ifdef INBA_HAS_CONCEPTS
    namespace concepts
    {
        using namespace traits;

        template<typename T, typename U>
        concept CvRefRemovedIsSame = cv_ref_removed_is_same<T, U>::value;

        template<typename T>
        concept WeaklyIncrementable = requires(T x)
        {
            ++x++;
        };
        template<typename T>
        concept Incrementable = requires(T x)
        {
            (++x)++;
        };
        template<typename T>
        concept StronglyIncrementable = Incrementable<T> && ! WeaklyIncrementable<T>;

        template<typename T>
        concept WeaklyDecrementable = requires(T x)
        {
            --x--;
        };
        template<typename T>
        concept Decrementable = requires(T x)
        {
            (--x)--;
        };
        template<typename T>
        concept StronglyDecrementable = Decrementable<T> && ! WeaklyDecrementable<T>;

        template<typename T>
        // todo: debatable excluding floating point; maybe add opt-in
        concept StronglyInDecrementable = StronglyIncrementable<T>&& StronglyDecrementable<T>; // && !std::is_floating_point_v<T>;

        template<typename T, typename U = T>
        concept AddableX = requires(T x, U y)
        {
            x + y;
            y + x;
        };
        template<typename T, typename U = T>
        concept SubtractableX = requires(T x, U y)
        {
            x - y;
            y - x;
        };
        template<typename T, typename U = T>
        concept MultiplyableX = requires(T x, U y)
        {
            x* y;
            y* x;
        };
        template<typename T, typename U = T>
        concept DividableX = requires(T x, U y)
        {
            x / y;
            y / x;
        };

        template<typename T, typename U = T>
        concept Addable = requires(T x, U y)
        {
            x + x;
            y + y;
            x + y;
            y + x;
        };
        template<typename T, typename U = T>
        concept Subtractable = requires(T x, U y)
        {
            x - x;
            y - y;
            x - y;
            y - x;
        };
        template<typename T, typename U = T>
        concept Multiplyable = requires(T x, U y)
        {
            x* x;
            y* y;
            x* y;
            y* x;
        };
        template<typename T, typename U = T>
        concept Dividable = requires(T x, U y)
        {
            x / x;
            y / y;
            x / y;
            y / x;
        };

        template<typename T, typename U = T>
        concept AddAndSubtractableX = AddableX<T, U>&& SubtractableX<T, U>;
        template<typename T, typename U = T>
        concept MultiplyAndDividableX = MultiplyableX<T, U>&& DividableX<T, U>;

        template<typename T, typename U = T>
        concept AddAndSubtractable = Addable<T, U>&& Subtractable<T, U>;
        template<typename T, typename U = T>
        concept MultiplyAndDividable = Multiplyable<T, U>&& Dividable<T, U>;

        template<typename T, typename U = T>
        concept CanDoArithmeticsX = AddableX<T, U>&& SubtractableX<T, U>&& MultiplyableX<T, U>&& DividableX<T, U>;

        template<typename T, typename U = T>
        concept CanDoArithmetics = Addable<T, U>&& Subtractable<T, U>&& Multiplyable<T, U>&& Dividable<T, U>;

        template<typename T>
        concept IsArrayIndex = (bool)is_array_index<T>::value;

        template<typename T, typename B = T>
        concept IndexedValueType = StronglyInDecrementable<T>&& AddableX<T, B>&& SubtractableX<T, B>&& IsArrayIndex<T>;
        template<typename T, typename B = T>
        concept CvRefRemovedIndexedValueType = IndexedValueType<typename remove_cvref<T>::type, typename remove_cvref<B>::type>;

        template<typename Index, typename IndexBase>
        concept IndexedValueBaseType = AddAndSubtractableX<Index, IndexBase>;

        template<typename T>
        concept HasIndexedValues = requires(T x)
        {
            x.value();
            x.index_base();
        };

        template<typename T>
        concept PureIndexedAssignableValueType = ! HasIndexedValues<T> && CvRefRemovedIndexedValueType<T>; // IndexedValueType<T> ;
        template<typename T>
        concept IndexedBaseCompatible = is_IndexedBase_compatible<T>::value;
        template<typename T>
        concept NotIndexedBaseCompatible = ! IndexedBaseCompatible<T>;

        // ranges (todo)

        // see std::ranges::equal_to - https://en.cppreference.com/w/cpp/utility/functional/ranges/equal_to
        template<typename T, typename U>
        concept CompDeclv = requires(T t, U u)
        {
            std::declval<T>() == std::declval<U>();
        };
        template<typename T, typename U>
        concept CompDeclvOrEqualityComparableWith = CompDeclv<T, U> || std::equality_comparable_with<T, U>;
    }

    using namespace concepts;
#endif
    using namespace traits;

#ifndef INBA_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
    namespace cnttp
    {
        /// <summary>
        /// Basic implementation of a helper type to work around the lack of class non-type template parameters.
        /// \ref Indexed expects a factory with a createIndexBase method which returns a base value instance.
        /// Implement your own factory for more advanced uses.
        /// </summary>
        /// <param name="B">base value type</param>
        /// <param name="N">base value number type</param>
        /// <param name="n">base value number value</param>
        template<typename B, typename N, N n>
        struct CreateIndexedHelper
        {
            static constexpr B createIndexBase()
            {
                return B{n};
            }
        };

        /// <summary>
        /// Convenience method to create \ref inba::cnttp::CreateIndexedHelper instances.
        /// </summary>
        /// <param name="B">base value type</param>
        /// <param name="N">base value number type</param>
        /// <param name="n">base value number value</param>
        template<typename N, N n, typename B = N>
        using createIndexedHelper = CreateIndexedHelper<B, N, n>;
    }

    template<INBA_COT(IndexedValueType) V, typename BaseFactory, typename DiffType>
    struct Indexed;

    namespace cnttp
    {
        template<typename N, N n, typename B = N, typename V = B, typename D = typename diff_type<V, B>::type>
        using createIndexedType = Indexed<V, CreateIndexedHelper<B, N, n>, D>;
    }
#endif
#ifndef INBA_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
    namespace cnttp
    {
        /// <summary>
        /// helper to invoke \ref inba::traits::get_as_signed_type for environments without class non-type template parameters.
        /// </summary>
        template<typename V, typename BaseFactory>
        struct DiffTypeHelper
        {
            typedef typename get_as_signed_type<typename diff_type<V, decltype(BaseFactory::createIndexBase())>::type>::type type;
        };
    }
#endif

    /// <summary>
    /// strong index type with base value
    /// </summary>
    /// <param name="V">underlying value type</param>
    /// <param name="Base">the value types basis value (only available with class non-type template parameters)</param>
    /// <param name="BaseFactory">helper type to work around the lack of class non-type template parameters</param>
    /// <param name="DiffType">value type difference type</param>
    template<INBA_COT(IndexedValueType) V = int,
#ifdef INBA_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
             auto Base = 0, typename DiffType = typename get_as_signed_type<typename diff_type<V, decltype(Base)>::type>::type
#else
             // sry.. not so nice
             typename BaseFactory = cnttp::CreateIndexedHelper<V, V, 0>,
             typename DiffType = typename cnttp::DiffTypeHelper<V, BaseFactory>::type
#endif
             >
    struct Indexed
    {
        INBA_PRE_CONCEPTS_CHECK(is_indexed_value_type<V>::value INBA_NTSA);
        INBA_PRE_CONCEPTS_CHECK(is_indexed_value_type<V>::value INBA_NTSA);

        using value_type = V;
        using diff_type = DiffType;
#ifdef INBA_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
        using base_type = decltype(Base);
        static constexpr base_type index_base_ = Base;
#else
        using base_type = decltype(BaseFactory::createIndexBase());
        static constexpr base_type index_base_ = BaseFactory::createIndexBase();
#endif
        static constexpr base_type const& index_base() noexcept
        {
            return index_base_;
        }

    private:
        value_type index_;

        template<INBA_COT(IndexedBaseCompatible) T
#ifndef INBA_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_IndexedBase_compatible<T>::value, bool>::type = true
#endif
                 >
        constexpr value_type convertFrom(T const& other) const
            noexcept(INBA_NOEXCEPT_OP(other.value() - other.index_base() + index_base_))
        {
            return other.value() - other.index_base() + index_base_;
        }

    public:
        constexpr Indexed() noexcept(INBA_NOEXCEPT_OP(value_type(index_base_)))
          : index_(index_base_)
        {}
        template<typename T
#ifndef INBA_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_pure_Indexed_assignable_value_type<T>::value &&
                                             std::is_floating_point<V>::value == std::is_floating_point<T>::value,
                                         bool>::type = true
#endif
                 >
#ifdef INBA_HAS_CONCEPTS
        requires(PureIndexedAssignableValueType<T>&& std::is_floating_point_v<V> == std::is_floating_point_v<T>)
#endif
            constexpr explicit Indexed(T index) noexcept(INBA_NOEXCEPT_OP(value_type(index)))
          : index_(index)
        {}
        template<INBA_COT(IndexedBaseCompatible) T
#ifndef INBA_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_IndexedBase_compatible<T>::value, bool>::type = true
#endif
                 >
        constexpr explicit Indexed(T const& other) noexcept(INBA_NOEXCEPT_OP(value_type(convertFrom(other))))
          : index_(convertFrom(other))
        {}
        template<
            typename T
#ifndef INBA_HAS_CONCEPTS
            ,
            typename std::enable_if<is_IndexedBase_compatible<T>::value && ! cv_ref_removed_is_same<T, Indexed>::value, bool>::type = true
#endif
            >
#ifdef INBA_HAS_CONCEPTS
        requires(IndexedBaseCompatible<T> && ! CvRefRemovedIsSame<T, Indexed>)
#endif
            constexpr explicit Indexed(T&& other) noexcept(INBA_NOEXCEPT_OP(value_type(convertFrom(other))))
          : index_(convertFrom(other))
        {}

        Indexed(Indexed const& other) = default;
        Indexed(Indexed&& other) = default;

        Indexed& operator=(Indexed const&) = default;
        Indexed& operator=(Indexed&& other) = default;

        template<INBA_COT(IndexedBaseCompatible) T
#ifndef INBA_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_IndexedBase_compatible<T>::value && ! cv_ref_removed_is_same<Indexed, T>::value, bool>::type =
                     true>
#else
        >
            requires(IndexedBaseCompatible<T> && !CvRefRemovedIsSame<Indexed, T>)
#endif
        INBA_CONSTEXPR_11 Indexed& operator=(T const& other) noexcept(INBA_NOEXCEPT_OP(operator=(Indexed(other))))
        {
            return operator=(Indexed(other));
        }
        template<typename T
#ifndef INBA_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_IndexedBase_compatible<T>::value && ! cv_ref_removed_is_same<Indexed, T>::value, bool>::type =
                     true>
#else
        >
            requires(IndexedBaseCompatible<T> && !CvRefRemovedIsSame<Indexed, T>)
#endif
        INBA_CONSTEXPR_11 Indexed& operator=(T&& other) noexcept(INBA_NOEXCEPT_OP(operator=(Indexed(other))))
        {
            return operator=(Indexed(other));
        }

        template<
            INBA_COT(IndexedBaseCompatible) T
#ifndef INBA_HAS_CONCEPTS
            ,
            typename std::enable_if<is_IndexedBase_compatible<T>::value && ! cv_ref_removed_is_same<T, Indexed>::value, bool>::type = true
#endif
            >
#ifdef INBA_HAS_CONCEPTS
        requires(IndexedBaseCompatible<T> && ! CvRefRemovedIsSame<T, Indexed>)
#endif
            constexpr explicit
            operator T() const noexcept(INBA_HAS_VALID_SFINAE_NOEXCEPT_OP(T(*this)))
        {
            return T(*this);
        }
        constexpr explicit operator value_type() const noexcept(INBA_NOEXCEPT_OP(value_type(index_ - index_base_)))
        {
            return index_ - index_base_;
        }
        template<typename T
#ifndef INBA_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_array_index<T>::value && ! is_IndexedBase_compatible<T>::value, bool>::type = true
#endif
                 >
#ifdef INBA_HAS_CONCEPTS
        requires(IsArrayIndex<T> && ! IndexedBaseCompatible<T>)
#endif
            // todo: debatable
            // explicit
            constexpr
            operator T() const noexcept(noexcept(T(static_cast<T>(index_) - index_base_)))
        {
            return static_cast<T>(index_) - index_base_;
        }

        constexpr Indexed const& operator+() const noexcept
        {
            return *this;
        }
        constexpr Indexed operator+(Indexed const& o) const noexcept(INBA_NOEXCEPT_OP(Indexed(index_ + o.index_)))
        {
            return Indexed(index_ + o.index_);
        }

        INBA_CONSTEXPR_11 Indexed& operator+=(Indexed const& o) & noexcept(INBA_NOEXCEPT_OP(index_ += o.index_))
        {
            index_ += o.index_;
            return *this;
        }
        INBA_CONSTEXPR_11 Indexed& operator+=(value_type v) & noexcept(INBA_NOEXCEPT_OP(index_ += v))
        {
            index_ += v;
            return *this;
        }

        constexpr Indexed operator-() const noexcept(INBA_NOEXCEPT_OP(Indexed(-index_)))
        {
            return Indexed(-index_);
        }
        constexpr diff_type operator-(Indexed const& o) const noexcept(INBA_NOEXCEPT_OP(diff_type(index_ - o.index_)))
        {
            return index_ - o.index_;
        }
        INBA_CONSTEXPR_11 Indexed& operator-=(Indexed const& o) & noexcept(INBA_NOEXCEPT_OP(index_ -= o.index_))
        {
            index_ -= o.index_;
            return *this;
        }
        INBA_CONSTEXPR_11 Indexed& operator-=(value_type v) & noexcept(INBA_NOEXCEPT_OP(index_ -= v))
        {
            index_ -= v;
            return *this;
        }
        INBA_CONSTEXPR_11 Indexed& operator*=(value_type i) & noexcept(INBA_NOEXCEPT_OP(index_ *= i))
        {
            index_ *= i;
            return *this;
        }
        INBA_CONSTEXPR_11 Indexed& operator/=(value_type i) & noexcept(INBA_NOEXCEPT_OP(index_ /= i))
        {
            index_ /= i;
            return *this;
        }

    private:
        INBA_CONSTEXPR_11 Indexed& operator*=(Indexed i) & noexcept;
        INBA_CONSTEXPR_11 Indexed& operator/=(Indexed i) & noexcept;

    public:
        INBA_CONSTEXPR_11 Indexed& operator++() & noexcept(INBA_NOEXCEPT_OP(++index_))
        {
            ++index_;
            return *this;
        }
        INBA_CONSTEXPR_11 Indexed operator++(int) & noexcept(INBA_NOEXCEPT_OP(Indexed(*this), ++(*this)))
        {
            Indexed result(*this);
            ++(*this);
            return result;
        }
        INBA_CONSTEXPR_11 Indexed& operator--() & noexcept(INBA_NOEXCEPT_OP(--index_))
        {
            --index_;
            return *this;
        }
        INBA_CONSTEXPR_11 Indexed operator--(int) & noexcept(INBA_NOEXCEPT_OP(Indexed(--(*this))))
        {
            Indexed result(*this);
            --(*this);
            return result;
        }

#ifdef INBA_HAS_SPACESHIP
        constexpr auto operator<=>(Indexed const& other) const = default;
        template<IndexedBaseCompatible T>
        requires(! std::same_as<T, Indexed>) constexpr auto operator<=>(T const& other) const
            noexcept(INBA_NOEXCEPT_OP(*this <=> Indexed(other)))
        {
            INBA_PRE_CONCEPTS_CHECK(is_IndexedBase_compatible<T>::value && ! std::is_same<T, Indexed>::value);
            return *this <=> Indexed(other);
        }
#else
        constexpr bool operator!=(Indexed const& other) const noexcept
        {
            return index_ != other.index_;
        }
        constexpr bool operator>=(Indexed const& other) const noexcept
        {
            return index_ >= other.index_;
        }
        constexpr bool operator<=(Indexed const& other) const noexcept
        {
            return index_ <= other.index_;
        }
        constexpr bool operator>(Indexed const& other) const noexcept
        {
            return index_ > other.index_;
        }
        constexpr bool operator<(Indexed const& other) const noexcept
        {
            return index_ < other.index_;
        }
#endif

        constexpr value_type const& value() const noexcept
        {
            return index_;
        }
// C2556: 'signed char &inba::Indexed<signed char,inba::CreateIndexedHelper<signed char,signed char,0>,int>::value(void) noexcept const' :
// overloaded function differs only by return type from 'const signed char &inba::Indexed<signed char,inba::CreateIndexedHelper<signed
// char,signed char,0>,int>::value(void) noexcept const'
#if _MSC_VER > 1900
        constexpr value_type& value() noexcept
        {
            return index_;
        }
#endif
    };

#ifndef INBA_HAS_DEFINITIONLESS_STATIC_CONSTEXPR_DATA_MEMBER
    template<typename V, typename BaseFactory, typename DiffType>
    constexpr typename Indexed<V, BaseFactory, DiffType>::base_type Indexed<V, BaseFactory, DiffType>::index_base_;
#endif

// \cond macro-verbose-255
#if defined(INBA_HAS_CONCEPTS) && defined(INBA_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS)
#define DEF_Indexed_Tpl template<IndexedValueType V, auto Base, typename DiffType>
#define DEF_Indexed_Type Indexed<V, Base, DiffType>
#else
#define DEF_Indexed_Tpl            \
    template<typename V,           \
             typename BaseFactory, \
             typename DiffType,    \
             typename std::enable_if<is_indexed_value_type<V>::value, bool>::type = true>
#define DEF_Indexed_Type Indexed<V, BaseFactory, DiffType>
#endif
    // \endcond

    DEF_Indexed_Tpl constexpr DEF_Indexed_Type operator+(
        DEF_Indexed_Type const& left,
        typename DEF_Indexed_Type::value_type right) noexcept(INBA_NOEXCEPT_OP(DEF_Indexed_Type(left.value() + right)))
    {
        return DEF_Indexed_Type(left.value() + right);
    }
    DEF_Indexed_Tpl constexpr DEF_Indexed_Type operator+(
        typename DEF_Indexed_Type::value_type const& left,
        DEF_Indexed_Type right) noexcept(INBA_NOEXCEPT_OP(DEF_Indexed_Type(left + right.value())))
    {
        return DEF_Indexed_Type(left + right.value());
    }
    DEF_Indexed_Tpl constexpr DEF_Indexed_Type operator-(
        DEF_Indexed_Type const& left,
        typename DEF_Indexed_Type::value_type right) noexcept(INBA_NOEXCEPT_OP(DEF_Indexed_Type(left.value() - right)))
    {
        return DEF_Indexed_Type(left.value() - right);
    }
    DEF_Indexed_Tpl constexpr DEF_Indexed_Type operator-(
        typename DEF_Indexed_Type::value_type const& left,
        DEF_Indexed_Type right) noexcept(INBA_NOEXCEPT_OP(DEF_Indexed_Type(left - right.value())))
    {
        return DEF_Indexed_Type(left - right.value());
    }
    DEF_Indexed_Tpl constexpr DEF_Indexed_Type operator*(
        DEF_Indexed_Type const& left,
        typename DEF_Indexed_Type::value_type right) noexcept(INBA_NOEXCEPT_OP(DEF_Indexed_Type(left.value() * right)))
    {
        return DEF_Indexed_Type(left.value() * right);
    }
    DEF_Indexed_Tpl constexpr DEF_Indexed_Type operator*(
        typename DEF_Indexed_Type::value_type const& left,
        DEF_Indexed_Type right) noexcept(INBA_NOEXCEPT_OP(DEF_Indexed_Type(left* right.value())))
    {
        return DEF_Indexed_Type(left * right.value());
    }
    DEF_Indexed_Tpl constexpr DEF_Indexed_Type operator/(
        DEF_Indexed_Type const& left,
        typename DEF_Indexed_Type::value_type right) noexcept(INBA_NOEXCEPT_OP(DEF_Indexed_Type(left.value() / right)))
    {
        return DEF_Indexed_Type(left.value() / right);
    }
    DEF_Indexed_Tpl constexpr DEF_Indexed_Type operator/(
        typename DEF_Indexed_Type::value_type const& left,
        DEF_Indexed_Type right) noexcept(INBA_NOEXCEPT_OP(DEF_Indexed_Type(left / right.value())))
    {
        return DEF_Indexed_Type(left / right.value());
    }

    template<INBA_COT(IndexedBaseCompatible) L, INBA_COT(IndexedBaseCompatible) R
#ifndef INBA_HAS_CONCEPTS
             ,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true
#endif
             >
    constexpr bool operator==(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(l.value() == L(r).value()))
    {
        return l.value() == L(r).value();
    }
    template<INBA_COT(IndexedBaseCompatible) L, INBA_COT(NotIndexedBaseCompatible) R
#ifndef INBA_HAS_CONCEPTS
             ,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true
#endif
             >
    /* todo: debatable */ INBA_COREQUIRES(! std::is_floating_point<R>::value) constexpr bool operator==(L const& l, R const& r) noexcept(
        INBA_NOEXCEPT_OP(static_cast<typename L::value_type>(l) == static_cast<typename L::value_type>(r)))
    {
        return static_cast<typename L::value_type>(l) == static_cast<typename L::value_type>(r);
    }
    template<INBA_COT(NotIndexedBaseCompatible) L, INBA_COT(IndexedBaseCompatible) R
#ifndef INBA_HAS_CONCEPTS
             ,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true
#endif
             >
    /* todo: debatable */ INBA_COREQUIRES(! std::is_floating_point_v<L>) constexpr bool operator==(L const& l, R const& r) noexcept(
        INBA_NOEXCEPT_OP(static_cast<typename R::value_type>(l) == static_cast<typename R::value_type>(r)))
    {
        return static_cast<typename R::value_type>(l) == static_cast<typename R::value_type>(r);
    }

#ifndef INBA_HAS_SPACESHIP
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true>
    constexpr bool operator!=(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(l == r))
    {
        return ! (l == r);
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator!=(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(l == r))
    {
        return ! (l == r);
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator!=(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(l == r))
    {
        return ! (l == r);
    }

    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! cv_ref_removed_is_same<L, R>::value,
                                     bool>::type = true>
    constexpr bool operator<(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(l < L(r)))
    {
        return l < L(r);
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator<(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(static_cast<typename L::value_type>(l) <
                                                                               static_cast<typename L::value_type>(r)))
    {
        return static_cast<typename L::value_type&&>(l) < static_cast<typename L::value_type>(r);
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator<(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(static_cast<typename R::value_type>(l) ==
                                                                               static_cast<typename R::value_type>(r)))
    {
        return static_cast<typename R::value_type>(l) < static_cast<typename R::value_type>(r);
    }

    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true>
    constexpr bool operator<=(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(l == r || l < r))
    {
        return l == r || l < r;
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator<=(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(l == r || l < r))
    {
        return l == r || l < r;
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator<=(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(l == r || l < r))
    {
        return l == r || l < r;
    }

    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true>
    constexpr bool operator>(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(! (l <= r)))
    {
        return ! (l <= r);
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator>(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(! (l <= r)))
    {
        return ! (l <= r);
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator>(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(! (l <= r)))
    {
        return ! (l <= r);
    }

    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true>
    constexpr bool operator>=(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(! (l < r)))
    {
        return ! (l < r);
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator>=(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(! (l < r)))
    {
        return ! (l < r);
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator>=(L const& l, R const& r) noexcept(INBA_NOEXCEPT_OP(! (l < r)))
    {
        return ! (l < r);
    }
#endif

#ifndef INDEX_BASE_DISABLE_DEFINE_LITERALS
#ifdef INBA_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
#define INDEX_BASE_DEFINE_LITERAL(mp_type, mp_index, mp_literal)                                   \
    inline Indexed<mp_type, mp_index> operator"" _##mp_literal##mp_index(unsigned long long int i) \
    {                                                                                              \
        return Indexed<mp_type, mp_index>(static_cast<mp_type>(i));                                \
    }
#else
/// <summary>
/// define a user defined literal for a specific \ref Indexed type instance and base value
/// </summary>
/// <param name="mp_type">base value type</param>
/// <param name="mp_index">base value</param>
/// <param name="mp_literal">literal name</param>
#define INDEX_BASE_DEFINE_LITERAL(mp_type, mp_index, mp_literal)                                                        \
    inline Indexed<mp_type, cnttp::CreateIndexedHelper<mp_type, mp_type, mp_index>> operator"" _##mp_literal##mp_index( \
        unsigned long long int i)                                                                                       \
    {                                                                                                                   \
        return Indexed<mp_type, cnttp::CreateIndexedHelper<mp_type, mp_type, mp_index>>(static_cast<mp_type>(i));       \
    }
#endif

// \cond macro-verbose-255
#define INDEX_BASE_DEFINE_LITERAL_PAIR_01(mp_type, mp_literal) \
    INDEX_BASE_DEFINE_LITERAL(mp_type, 0, mp_literal);         \
    INDEX_BASE_DEFINE_LITERAL(mp_type, 1, mp_literal);
// \endcond
#else
// \cond macro-verbose-255
#define INDEX_BASE_DEFINE_LITERAL_PAIR_01(mp_type, mp_literal)
// \endcond
#endif

#ifndef INDEX_BASE_DISABLE_DEFINE_USINGS
#ifdef INBA_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
#define INDEX_BASE_DEFINE_USING(mp_type, mp_index, mp_literal) using Indexed_##mp_literal##mp_index = Indexed<mp_type, mp_index>
#else
/// <summary>
/// define a typedef for a specific \ref Indexed instance type and base value
/// </summary>
/// <param name="mp_type">base value type</param>
/// <param name="mp_index">base value</param>
/// <param name="mp_literal">literal name</param>
#define INDEX_BASE_DEFINE_USING(mp_type, mp_index, mp_literal) \
    using Indexed_##mp_literal##mp_index = Indexed<mp_type, cnttp::CreateIndexedHelper<mp_type, mp_type, mp_index>>
#endif
// \cond macro-verbose-255
#define INDEX_BASE_DEFINE_USING_PAIR_01(mp_type, mp_literal) \
    INDEX_BASE_DEFINE_USING(mp_type, 0, mp_literal);         \
    INDEX_BASE_DEFINE_USING(mp_type, 1, mp_literal);
// \endcond
#else
// \cond macro-verbose-255
#define INDEX_BASE_DEFINE_USING_PAIR_01(mp_type, mp_literal)
// \endcond
#endif

/// <summary>
/// define a typedef and a user defined literal for a specific \ref Indexed type instance for bases zero and one
/// </summary>
/// <param name="mp_type">base value type</param>
/// <param name="mp_literal">literal name</param>
#define INDEX_BASE_DEFINE_ALL(mp_type, mp_literal)          \
    INDEX_BASE_DEFINE_LITERAL_PAIR_01(mp_type, mp_literal); \
    INDEX_BASE_DEFINE_USING_PAIR_01(mp_type, mp_literal);

    INDEX_BASE_DEFINE_ALL(signed char, ci);
    INDEX_BASE_DEFINE_ALL(unsigned char, uci);
    INDEX_BASE_DEFINE_ALL(signed short, si);
    INDEX_BASE_DEFINE_ALL(unsigned short, usi);
    INDEX_BASE_DEFINE_ALL(signed int, i);
    INDEX_BASE_DEFINE_ALL(unsigned int, ui);
    INDEX_BASE_DEFINE_ALL(signed long, l);
    INDEX_BASE_DEFINE_ALL(unsigned long, ul);
    INDEX_BASE_DEFINE_ALL(signed long long, ll);
    INDEX_BASE_DEFINE_ALL(unsigned long long, ull);
    INDEX_BASE_DEFINE_ALL(std::ptrdiff_t, z);
    INDEX_BASE_DEFINE_ALL(std::size_t, uz);
}

#if _MSC_VER <= 1900
#pragma warning(pop)
#endif

#endif
