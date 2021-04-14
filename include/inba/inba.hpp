/// \file inba.hpp

#pragma once
#ifndef INCLUDED_INBA_INBA
// \cond macro-verbose-255
#define INCLUDED_INBA_INBA

#include <gympl/gympl.hpp>
#include <gympl/platform.hpp>

// \endcond

#include <algorithm>
#include <type_traits>
#ifdef GYMPL_HAS_SPACESHIP
#include <compare>
#endif

#if _MSC_VER <= 1900
#pragma warning(push)
#pragma warning(disable : 4814) // MS
#endif

namespace inba
{
    namespace traits
    {
        using namespace gympl::compatibility;
        using namespace gympl::mpl;
        using namespace gympl::traits;

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
          : bool_constant<is_indexed_value_type<typename get_Indexed_value_type<T, NotMyType>::type,
                                                typename get_Indexed_value_base_type<T, NotMyType>::type>::value>
        {};
    }

#ifdef GYMPL_HAS_CONCEPTS
    namespace concepts
    {
        using namespace gympl::traits;
        using namespace gympl::concepts;

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
    }

    using namespace concepts;
#endif
    using namespace traits;

#ifndef GYMPL_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
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

    template<GYMPL_COT(IndexedValueType) V, typename BaseFactory, typename DiffType>
    struct Indexed;

    namespace cnttp
    {
        template<typename N, N n, typename B = N, typename V = B, typename D = typename diff_type<V, B>::type>
        using createIndexedType = Indexed<V, CreateIndexedHelper<B, N, n>, D>;
    }
#endif
#ifndef GYMPL_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
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
    template<GYMPL_COT(IndexedValueType) V = int,
#ifdef GYMPL_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
             auto Base = 0, typename DiffType = typename get_as_signed_type<typename diff_type<V, decltype(Base)>::type>::type
#else
             // sry.. not so nice
             typename BaseFactory = cnttp::CreateIndexedHelper<V, V, 0>,
             typename DiffType = typename cnttp::DiffTypeHelper<V, BaseFactory>::type
#endif
             >
    struct Indexed
    {
        GYMPL_PRE_CONCEPTS_CHECK(is_indexed_value_type<V>::value GYMPL_NTSA);
        GYMPL_PRE_CONCEPTS_CHECK(is_indexed_value_type<V>::value GYMPL_NTSA);

        using value_type = V;
        using diff_type = DiffType;
#ifdef GYMPL_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
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

        template<GYMPL_COT(IndexedBaseCompatible) T
#ifndef GYMPL_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_IndexedBase_compatible<T>::value, bool>::type = true
#endif
                 >
        constexpr value_type convertFrom(T const& other) const
            noexcept(GYMPL_NOEXCEPT_OP(other.value() - other.index_base() + index_base_))
        {
            return other.value() - other.index_base() + index_base_;
        }

    public:
        constexpr Indexed() noexcept(GYMPL_NOEXCEPT_OP(value_type(index_base_)))
          : index_(index_base_)
        {}
        template<typename T
#ifndef GYMPL_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_pure_Indexed_assignable_value_type<T>::value &&
                                             std::is_floating_point<V>::value == std::is_floating_point<T>::value,
                                         bool>::type = true
#endif
                 >
#ifdef GYMPL_HAS_CONCEPTS
        requires(PureIndexedAssignableValueType<T>&& std::is_floating_point_v<V> == std::is_floating_point_v<T>)
#endif
            constexpr explicit Indexed(T index) noexcept(GYMPL_NOEXCEPT_OP(value_type(index)))
          : index_(index)
        {}
        template<GYMPL_COT(IndexedBaseCompatible) T
#ifndef GYMPL_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_IndexedBase_compatible<T>::value, bool>::type = true
#endif
                 >
        constexpr explicit Indexed(T const& other) noexcept(GYMPL_NOEXCEPT_OP(value_type(convertFrom(other))))
          : index_(convertFrom(other))
        {}
        template<
            typename T
#ifndef GYMPL_HAS_CONCEPTS
            ,
            typename std::enable_if<is_IndexedBase_compatible<T>::value && ! cv_ref_removed_is_same<T, Indexed>::value, bool>::type = true
#endif
            >
#ifdef GYMPL_HAS_CONCEPTS
        requires(IndexedBaseCompatible<T> && ! CvRefRemovedIsSame<T, Indexed>)
#endif
            constexpr explicit Indexed(T&& other) noexcept(GYMPL_NOEXCEPT_OP(value_type(convertFrom(other))))
          : index_(convertFrom(other))
        {}

        Indexed(Indexed const& other) = default;
        Indexed(Indexed&& other) = default;

        Indexed& operator=(Indexed const&) = default;
        Indexed& operator=(Indexed&& other) = default;

        template<GYMPL_COT(IndexedBaseCompatible) T
#ifndef GYMPL_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_IndexedBase_compatible<T>::value && ! cv_ref_removed_is_same<Indexed, T>::value, bool>::type =
                     true>
#else
        >
            requires(IndexedBaseCompatible<T> && !CvRefRemovedIsSame<Indexed, T>)
#endif
        GYMPL_CONSTEXPR_11 Indexed& operator=(T const& other) noexcept(GYMPL_NOEXCEPT_OP(operator=(Indexed(other))))
        {
            return operator=(Indexed(other));
        }
        template<typename T
#ifndef GYMPL_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_IndexedBase_compatible<T>::value && ! cv_ref_removed_is_same<Indexed, T>::value, bool>::type =
                     true>
#else
        >
            requires(IndexedBaseCompatible<T> && !CvRefRemovedIsSame<Indexed, T>)
#endif
        GYMPL_CONSTEXPR_11 Indexed& operator=(T&& other) noexcept(GYMPL_NOEXCEPT_OP(operator=(Indexed(other))))
        {
            return operator=(Indexed(other));
        }

        template<
            GYMPL_COT(IndexedBaseCompatible) T
#ifndef GYMPL_HAS_CONCEPTS
            ,
            typename std::enable_if<is_IndexedBase_compatible<T>::value && ! cv_ref_removed_is_same<T, Indexed>::value, bool>::type = true
#endif
            >
#ifdef GYMPL_HAS_CONCEPTS
        requires(IndexedBaseCompatible<T> && ! CvRefRemovedIsSame<T, Indexed>)
#endif
            constexpr explicit
            operator T() const noexcept(GYMPL_HAS_VALID_SFINAE_NOEXCEPT_OP(T(*this)))
        {
            return T(*this);
        }
        constexpr explicit operator value_type() const noexcept(GYMPL_NOEXCEPT_OP(value_type(index_ - index_base_)))
        {
            return index_ - index_base_;
        }
        template<typename T
#ifndef GYMPL_HAS_CONCEPTS
                 ,
                 typename std::enable_if<is_array_index<T>::value && ! is_IndexedBase_compatible<T>::value, bool>::type = true
#endif
                 >
#ifdef GYMPL_HAS_CONCEPTS
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
        constexpr Indexed operator+(Indexed const& o) const noexcept(GYMPL_NOEXCEPT_OP(Indexed(index_ + o.index_)))
        {
            return Indexed(index_ + o.index_);
        }

        GYMPL_CONSTEXPR_11 Indexed& operator+=(Indexed const& o) & noexcept(GYMPL_NOEXCEPT_OP(index_ += o.index_))
        {
            index_ += o.index_;
            return *this;
        }
        GYMPL_CONSTEXPR_11 Indexed& operator+=(value_type v) & noexcept(GYMPL_NOEXCEPT_OP(index_ += v))
        {
            index_ += v;
            return *this;
        }

        constexpr Indexed operator-() const noexcept(GYMPL_NOEXCEPT_OP(Indexed(-index_)))
        {
            return Indexed(-index_);
        }
        constexpr diff_type operator-(Indexed const& o) const noexcept(GYMPL_NOEXCEPT_OP(diff_type(index_ - o.index_)))
        {
            return index_ - o.index_;
        }
        GYMPL_CONSTEXPR_11 Indexed& operator-=(Indexed const& o) & noexcept(GYMPL_NOEXCEPT_OP(index_ -= o.index_))
        {
            index_ -= o.index_;
            return *this;
        }
        GYMPL_CONSTEXPR_11 Indexed& operator-=(value_type v) & noexcept(GYMPL_NOEXCEPT_OP(index_ -= v))
        {
            index_ -= v;
            return *this;
        }
        GYMPL_CONSTEXPR_11 Indexed& operator*=(value_type i) & noexcept(GYMPL_NOEXCEPT_OP(index_ *= i))
        {
            index_ *= i;
            return *this;
        }
        GYMPL_CONSTEXPR_11 Indexed& operator/=(value_type i) & noexcept(GYMPL_NOEXCEPT_OP(index_ /= i))
        {
            index_ /= i;
            return *this;
        }

    private:
        GYMPL_CONSTEXPR_11 Indexed& operator*=(Indexed i) & noexcept;
        GYMPL_CONSTEXPR_11 Indexed& operator/=(Indexed i) & noexcept;

    public:
        GYMPL_CONSTEXPR_11 Indexed& operator++() & noexcept(GYMPL_NOEXCEPT_OP(++index_))
        {
            ++index_;
            return *this;
        }
        GYMPL_CONSTEXPR_11 Indexed operator++(int) & noexcept(GYMPL_NOEXCEPT_OP(Indexed(*this), ++(*this)))
        {
            Indexed result(*this);
            ++(*this);
            return result;
        }
        GYMPL_CONSTEXPR_11 Indexed& operator--() & noexcept(GYMPL_NOEXCEPT_OP(--index_))
        {
            --index_;
            return *this;
        }
        GYMPL_CONSTEXPR_11 Indexed operator--(int) & noexcept(GYMPL_NOEXCEPT_OP(Indexed(--(*this))))
        {
            Indexed result(*this);
            --(*this);
            return result;
        }

#ifdef GYMPL_HAS_SPACESHIP
        constexpr auto operator<=>(Indexed const& other) const = default;
        template<IndexedBaseCompatible T>
        requires(! std::same_as<T, Indexed>) constexpr auto operator<=>(T const& other) const
            noexcept(GYMPL_NOEXCEPT_OP(*this <=> Indexed(other)))
        {
            GYMPL_PRE_CONCEPTS_CHECK(is_IndexedBase_compatible<T>::value && ! std::is_same<T, Indexed>::value);
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

#ifndef GYMPL_HAS_DEFINITIONLESS_STATIC_CONSTEXPR_DATA_MEMBER
    template<typename V, typename BaseFactory, typename DiffType>
    constexpr typename Indexed<V, BaseFactory, DiffType>::base_type Indexed<V, BaseFactory, DiffType>::index_base_;
#endif

// \cond macro-verbose-255
#if defined(GYMPL_HAS_CONCEPTS) && defined(GYMPL_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS)
#define INBA_DEF_Indexed_Tpl template<IndexedValueType V, auto Base, typename DiffType>
#define INBA_DEF_Indexed_Type Indexed<V, Base, DiffType>
#else
#define INBA_DEF_Indexed_Tpl       \
    template<typename V,           \
             typename BaseFactory, \
             typename DiffType,    \
             typename std::enable_if<is_indexed_value_type<V>::value, bool>::type = true>
#define INBA_DEF_Indexed_Type Indexed<V, BaseFactory, DiffType>
#endif
    // \endcond

    INBA_DEF_Indexed_Tpl constexpr INBA_DEF_Indexed_Type operator+(
        INBA_DEF_Indexed_Type const& left,
        typename INBA_DEF_Indexed_Type::value_type right) noexcept(GYMPL_NOEXCEPT_OP(INBA_DEF_Indexed_Type(left.value() + right)))
    {
        return INBA_DEF_Indexed_Type(left.value() + right);
    }
    INBA_DEF_Indexed_Tpl constexpr INBA_DEF_Indexed_Type operator+(
        typename INBA_DEF_Indexed_Type::value_type const& left,
        INBA_DEF_Indexed_Type right) noexcept(GYMPL_NOEXCEPT_OP(INBA_DEF_Indexed_Type(left + right.value())))
    {
        return INBA_DEF_Indexed_Type(left + right.value());
    }
    INBA_DEF_Indexed_Tpl constexpr INBA_DEF_Indexed_Type operator-(
        INBA_DEF_Indexed_Type const& left,
        typename INBA_DEF_Indexed_Type::value_type right) noexcept(GYMPL_NOEXCEPT_OP(INBA_DEF_Indexed_Type(left.value() - right)))
    {
        return INBA_DEF_Indexed_Type(left.value() - right);
    }
    INBA_DEF_Indexed_Tpl constexpr INBA_DEF_Indexed_Type operator-(
        typename INBA_DEF_Indexed_Type::value_type const& left,
        INBA_DEF_Indexed_Type right) noexcept(GYMPL_NOEXCEPT_OP(INBA_DEF_Indexed_Type(left - right.value())))
    {
        return INBA_DEF_Indexed_Type(left - right.value());
    }
    INBA_DEF_Indexed_Tpl constexpr INBA_DEF_Indexed_Type operator*(
        INBA_DEF_Indexed_Type const& left,
        typename INBA_DEF_Indexed_Type::value_type right) noexcept(GYMPL_NOEXCEPT_OP(INBA_DEF_Indexed_Type(left.value() * right)))
    {
        return INBA_DEF_Indexed_Type(left.value() * right);
    }
    INBA_DEF_Indexed_Tpl constexpr INBA_DEF_Indexed_Type operator*(
        typename INBA_DEF_Indexed_Type::value_type const& left,
        INBA_DEF_Indexed_Type right) noexcept(GYMPL_NOEXCEPT_OP(INBA_DEF_Indexed_Type(left* right.value())))
    {
        return INBA_DEF_Indexed_Type(left * right.value());
    }
    INBA_DEF_Indexed_Tpl constexpr INBA_DEF_Indexed_Type operator/(
        INBA_DEF_Indexed_Type const& left,
        typename INBA_DEF_Indexed_Type::value_type right) noexcept(GYMPL_NOEXCEPT_OP(INBA_DEF_Indexed_Type(left.value() / right)))
    {
        return INBA_DEF_Indexed_Type(left.value() / right);
    }
    INBA_DEF_Indexed_Tpl constexpr INBA_DEF_Indexed_Type operator/(
        typename INBA_DEF_Indexed_Type::value_type const& left,
        INBA_DEF_Indexed_Type right) noexcept(GYMPL_NOEXCEPT_OP(INBA_DEF_Indexed_Type(left / right.value())))
    {
        return INBA_DEF_Indexed_Type(left / right.value());
    }

    template<GYMPL_COT(IndexedBaseCompatible) L, GYMPL_COT(IndexedBaseCompatible) R
#ifndef GYMPL_HAS_CONCEPTS
             ,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true
#endif
             >
    constexpr bool operator==(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(l.value() == L(r).value()))
    {
        return l.value() == L(r).value();
    }
    template<GYMPL_COT(IndexedBaseCompatible) L, GYMPL_COT(NotIndexedBaseCompatible) R
#ifndef GYMPL_HAS_CONCEPTS
             ,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true
#endif
             >
    /* todo: debatable */ GYMPL_COREQUIRES(! std::is_floating_point<R>::value) constexpr bool operator==(L const& l, R const& r) noexcept(
        GYMPL_NOEXCEPT_OP(static_cast<typename L::value_type>(l) == static_cast<typename L::value_type>(r)))
    {
        return static_cast<typename L::value_type>(l) == static_cast<typename L::value_type>(r);
    }
    template<GYMPL_COT(NotIndexedBaseCompatible) L, GYMPL_COT(IndexedBaseCompatible) R
#ifndef GYMPL_HAS_CONCEPTS
             ,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true
#endif
             >
    /* todo: debatable */ GYMPL_COREQUIRES(! std::is_floating_point_v<L>) constexpr bool operator==(L const& l, R const& r) noexcept(
        GYMPL_NOEXCEPT_OP(static_cast<typename R::value_type>(l) == static_cast<typename R::value_type>(r)))
    {
        return static_cast<typename R::value_type>(l) == static_cast<typename R::value_type>(r);
    }

#ifndef GYMPL_HAS_SPACESHIP
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true>
    constexpr bool operator!=(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(l == r))
    {
        return ! (l == r);
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator!=(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(l == r))
    {
        return ! (l == r);
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator!=(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(l == r))
    {
        return ! (l == r);
    }

    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! cv_ref_removed_is_same<L, R>::value,
                                     bool>::type = true>
    constexpr bool operator<(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(l < L(r)))
    {
        return l < L(r);
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator<(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(static_cast<typename L::value_type>(l) <
                                                                                static_cast<typename L::value_type>(r)))
    {
        return static_cast<typename L::value_type&&>(l) < static_cast<typename L::value_type>(r);
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator<(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(static_cast<typename R::value_type>(l) ==
                                                                                static_cast<typename R::value_type>(r)))
    {
        return static_cast<typename R::value_type>(l) < static_cast<typename R::value_type>(r);
    }

    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true>
    constexpr bool operator<=(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(l == r || l < r))
    {
        return l == r || l < r;
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator<=(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(l == r || l < r))
    {
        return l == r || l < r;
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator<=(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(l == r || l < r))
    {
        return l == r || l < r;
    }

    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true>
    constexpr bool operator>(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(! (l <= r)))
    {
        return ! (l <= r);
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator>(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(! (l <= r)))
    {
        return ! (l <= r);
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator>(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(! (l <= r)))
    {
        return ! (l <= r);
    }

    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value, bool>::type = true>
    constexpr bool operator>=(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(! (l < r)))
    {
        return ! (l < r);
    }
    template<typename L, typename R,
             typename std::enable_if<is_IndexedBase_compatible<L>::value && ! is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<R>::value,
                                     bool>::type = true>
    constexpr bool operator>=(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(! (l < r)))
    {
        return ! (l < r);
    }
    template<typename L, typename R,
             typename std::enable_if<! is_IndexedBase_compatible<L>::value && is_IndexedBase_compatible<R>::value &&
                                         ! std::is_floating_point<L>::value,
                                     bool>::type = true>
    constexpr bool operator>=(L const& l, R const& r) noexcept(GYMPL_NOEXCEPT_OP(! (l < r)))
    {
        return ! (l < r);
    }
#endif

#ifndef INDEX_BASE_DISABLE_DEFINE_LITERALS
#ifdef GYMPL_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
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
#ifdef GYMPL_HAS_CLASS_TYPE_NON_TYPE_TEMPLATE_PARAMS
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
