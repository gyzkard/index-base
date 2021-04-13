#include "testing.hpp"

#include <type_traits>

//#define INDEX_BASE_DISABLE_DEFINE_LITERALS
//#define INDEX_BASE_DISABLE_DEFINE_USINGS
#include <inba/inba.hpp>

_Pragma("clang diagnostic push")
    //_Pragma("clang diagnostic ignored \"-Wunevaluated-expression\"")

    namespace
{
    void impl_concept_trait_checks()
    {
        using namespace inba;

        static_assert(! std::is_trivial<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_trivially_copyable<Indexed_i0>::value INBA_NTSA);
#ifdef INBA_HAS_CPP17
        static_assert(! std::is_aggregate<Indexed_i0>::value INBA_NTSA);
#endif

        static_assert(std::is_constructible<Indexed_i0, int>::value INBA_NTSA);
        static_assert(! std::is_trivially_constructible<Indexed_i0, int>::value INBA_NTSA);
        static_assert(std::is_nothrow_constructible<Indexed_i0>::value INBA_NTSA);
        // todo: not providing an implicit copy ctor is debatable
        static_assert(std::is_constructible<Indexed_i0, Indexed_i1>::value INBA_NTSA);
        static_assert(! std::is_trivially_constructible<Indexed_i0, Indexed_i1>::value INBA_NTSA);
        static_assert(
#ifndef INBA_HAS_NOEXCEPT_OP
            !
#endif
            std::is_nothrow_constructible<Indexed_i0, Indexed_i1>::value INBA_NTSA);

        static_assert(std::is_default_constructible<Indexed_i0>::value INBA_NTSA);
        static_assert(! std::is_trivially_default_constructible<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_nothrow_default_constructible<Indexed_i0>::value INBA_NTSA);

        static_assert(std::is_default_constructible<Indexed_i0>::value INBA_NTSA);
        static_assert(! std::is_trivially_default_constructible<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_nothrow_default_constructible<Indexed_i0>::value INBA_NTSA);

        static_assert(std::is_copy_constructible<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_trivially_copy_constructible<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_nothrow_copy_constructible<Indexed_i0>::value INBA_NTSA);

        static_assert(std::is_move_constructible<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_trivially_move_constructible<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_nothrow_move_constructible<Indexed_i0>::value INBA_NTSA);

        static_assert(std::is_assignable<Indexed_i0, Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_trivially_assignable<Indexed_i0, Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_nothrow_assignable<Indexed_i0, Indexed_i0>::value INBA_NTSA);

        static_assert(std::is_assignable<Indexed_i0, Indexed_i1>::value INBA_NTSA);
        static_assert(! std::is_trivially_assignable<Indexed_i0, Indexed_i1>::value INBA_NTSA);
        static_assert(
#ifndef INBA_HAS_NOEXCEPT_OP
            !
#endif
            std::is_nothrow_assignable<Indexed_i0, Indexed_i1>::value INBA_NTSA);

        static_assert(! std::is_assignable<Indexed_i0, int>::value INBA_NTSA);
        static_assert(! std::is_trivially_assignable<Indexed_i0, int>::value INBA_NTSA);
        static_assert(! std::is_nothrow_assignable<Indexed_i0, int>::value INBA_NTSA);

        static_assert(std::is_copy_assignable<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_trivially_copy_assignable<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_nothrow_copy_assignable<Indexed_i0>::value INBA_NTSA);

        static_assert(std::is_move_assignable<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_trivially_move_assignable<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_nothrow_move_assignable<Indexed_i0>::value INBA_NTSA);

        static_assert(std::is_destructible<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_trivially_destructible<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_nothrow_destructible<Indexed_i0>::value INBA_NTSA);

#ifdef INBA_HAS_CPP17
        static_assert(std::is_swappable<Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_nothrow_swappable<Indexed_i0>::value INBA_NTSA);
        static_assert(! std::is_swappable_with<Indexed_i0, Indexed_i1>::value INBA_NTSA);
#endif

        // todo: not providing an implicit copy ctor is debatable
        static_assert(! std::is_convertible<Indexed_i1, Indexed_i0>::value INBA_NTSA);
#ifdef INBA_HAS_is_nothrow_convertible
        static_assert(! std::is_nothrow_convertible<Indexed_i1, Indexed_i0>::value);
#endif

#ifdef INBA_HAS_CONCEPTS
        static_assert(std::destructible<Indexed_i0>);
        static_assert(std::default_initializable<Indexed_i0>);
        static_assert(! std::assignable_from<Indexed_i0, int>);
#endif

        // todo: implicit cast to integer is debatable
        static_assert(std::is_convertible<Indexed_i0, int>::value INBA_NTSA);
#ifdef INBA_HAS_CONCEPTS
        static_assert(std::regular<Indexed_i0>);
        static_assert(std::totally_ordered<Indexed_i0>);
#endif

        static_assert(is_addable<Indexed_i0>::value INBA_NTSA);
        static_assert(is_addable_x<Indexed_i0, Indexed_i0::value_type>::value INBA_NTSA);
        static_assert(! is_addable_x<Indexed_i0, Indexed_i1>::value INBA_NTSA);
        static_assert(is_subtractable<Indexed_i0>::value INBA_NTSA);
        static_assert(is_subtractable_x<Indexed_i0, Indexed_i1::value_type>::value INBA_NTSA);
        static_assert(! is_subtractable_x<Indexed_i0, Indexed_i1>::value INBA_NTSA);
        static_assert(! is_multiplyable<Indexed_i0>::value INBA_NTSA);
        static_assert(is_multiplyable_x<Indexed_i0, Indexed_i1::value_type>::value INBA_NTSA);
        static_assert(! is_multiplyable_x<Indexed_i0, Indexed_i1>::value INBA_NTSA);
        static_assert(! is_dividable<Indexed_i0>::value INBA_NTSA);
        static_assert(is_dividable_x<Indexed_i0, Indexed_i1::value_type>::value INBA_NTSA);
        static_assert(! is_dividable_x<Indexed_i0, Indexed_i1>::value INBA_NTSA);

        static_assert(! is_weakly_incrementable<Indexed_i0>::value INBA_NTSA);
        static_assert(is_strongly_incrementable<Indexed_i0>::value INBA_NTSA);
        static_assert(! is_weakly_decrementable<Indexed_i0>::value INBA_NTSA);
        static_assert(is_strongly_decrementable<Indexed_i0>::value INBA_NTSA);
        static_assert(is_strongly_in_and_decrementable<Indexed_i0>::value INBA_NTSA);

        static_assert(is_add_and_subtractable<Indexed_i0>::value INBA_NTSA);
        static_assert(! is_add_and_subtractable<Indexed_i0, Indexed_i1>::value INBA_NTSA);

        static_assert(! can_do_arithmetics<Indexed_i0>::value INBA_NTSA);
        static_assert(can_do_arithmetics_x<Indexed_i0, Indexed_i0::value_type>::value INBA_NTSA);
        static_assert(! can_do_arithmetics_x<Indexed_i0, Indexed_i1>::value INBA_NTSA);
        static_assert(std::is_convertible<Indexed_i0, Indexed_i0::value_type>::value ==
                      can_do_arithmetics_x<Indexed_i0, Indexed_i0::value_type>::value INBA_NTSA);

        static_assert(std::is_same<diff_type<unsigned int, unsigned int>::type, unsigned int>::value INBA_NTSA);
        static_assert(std::is_same<diff_type<Indexed_i0, int>::type, Indexed_i0>::value INBA_NTSA);
        static_assert(std::is_same<diff_type<Indexed_ui0, Indexed_ui0>::type, int>::value INBA_NTSA);
        static_assert(std::is_same<diff_type<Indexed_ui0, Indexed_ui0>::type, Indexed_ui0::diff_type>::value INBA_NTSA);

        static_assert(is_array_index<Indexed_i0>::value INBA_NTSA);

        static_assert(std::is_convertible<Indexed_i0, int>::value == is_indexed_value_type<Indexed_i0>::value INBA_NTSA);
        static_assert(is_indexed_value_type<Indexed_i0>::value INBA_NTSA);
        static_assert(is_cv_ref_removed_indexed_value_type<Indexed_i0>::value INBA_NTSA);

        static_assert(is_indexed_value_base_type<remove_cvref<decltype(Indexed_i0().value())>::type,
                                                 std::remove_cv<decltype(Indexed_i0().index_base())>::type>::value INBA_NTSA);
        static_assert(is_indexed_value_base_type<Indexed_i0, int>::value INBA_NTSA);
        // todo?
        static_assert(! is_indexed_value_base_type<Indexed_i0, float>::value INBA_NTSA);

        static_assert(has_Indexed_values<Indexed_i0>::value INBA_NTSA);

        static_assert(std::is_same<get_Indexed_value_type<Indexed_i0, NotMyType>::type, int>::value INBA_NTSA);
        static_assert(std::is_same<get_Indexed_value_base_type<Indexed_i0, NotMyType>::type, int>::value INBA_NTSA);
        static_assert(std::is_same<get_Indexed_value_type<Indexed_i0, NotMyType>::type, Indexed_i0::value_type>::value INBA_NTSA);
        static_assert(std::is_same<get_Indexed_value_base_type<Indexed_i0, NotMyType>::type, Indexed_i0::base_type>::value INBA_NTSA);

        static_assert(! is_pure_Indexed_assignable_value_type<Indexed_i0>::value INBA_NTSA);

        static_assert(is_IndexedBase_compatible<Indexed_i0>::value INBA_NTSA);
        static_assert(is_IndexedBase_compatible<Indexed_uci1>::value INBA_NTSA);
    }
    void impl_concept_concept_checks()
    {
        using namespace inba;

#ifdef INBA_HAS_CONCEPTS
        static_assert(StronglyInDecrementable<Indexed_i0>);

        static_assert(Addable<Indexed_i0>);
        static_assert(AddableX<Indexed_i0, Indexed_i0::value_type>);
        static_assert(! AddableX<Indexed_i0, Indexed_i1>);
        static_assert(Subtractable<Indexed_i0>);
        static_assert(SubtractableX<Indexed_i0, Indexed_i0::value_type>);
        static_assert(! SubtractableX<Indexed_i0, Indexed_i1>);
        static_assert(! Multiplyable<Indexed_i0>);
        static_assert(MultiplyableX<Indexed_i0, Indexed_i0::value_type>);
        static_assert(! MultiplyableX<Indexed_i0, Indexed_i1>);
        static_assert(! Dividable<Indexed_i0>);
        static_assert(DividableX<Indexed_i0, Indexed_i0::value_type>);
        static_assert(! DividableX<Indexed_i0, Indexed_i1>);
        static_assert(! CanDoArithmetics<Indexed_i0>);
        static_assert(CanDoArithmeticsX<Indexed_i0, Indexed_i0::value_type>);
        static_assert(! CanDoArithmetics<Indexed_i0, Indexed_i1>);
        static_assert(std::is_convertible<Indexed_i0, Indexed_i0::value_type>::value ==
                      CanDoArithmeticsX<Indexed_i0, Indexed_i0::value_type>);

        // todo: implicit cast to integer is debatable
        static_assert(IsArrayIndex<Indexed_i0>);

        // todo: implicit cast to integer is debatable
        static_assert(std::is_convertible<Indexed_i0, int>::value == IndexedValueType<Indexed_i0>);
        static_assert(IndexedValueType<Indexed_i0>);
        static_assert(CvRefRemovedIndexedValueType<Indexed_i0>);

        static_assert(IndexedValueType<std::remove_cvref<decltype(Indexed_i0().value())>::type>);
        static_assert(IndexedValueBaseType<std::remove_cvref<decltype(Indexed_i0().value())>::type,
                                           std::remove_cv<decltype(Indexed_i0().index_base())>::type>);
        static_assert(IndexedValueBaseType<Indexed_i0, int>);
        // todo?
        static_assert(! IndexedValueBaseType<Indexed_i0, float>);

        static_assert(HasIndexedValues<Indexed_i0>);

        static_assert(! PureIndexedAssignableValueType<Indexed_i0>);

        static_assert(IndexedBaseCompatible<Indexed_i0>);
        static_assert(IndexedBaseCompatible<Indexed_uci1>);
#endif
    }
}

TEST_MAIN_START
{
    return EXIT_SUCCESS;
}
TEST_MAIN_END

_Pragma("clang diagnostic pop")
