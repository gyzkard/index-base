#include "testing.hpp"

#include <type_traits>

//#define INDEX_BASE_DISABLE_DEFINE_LITERALS
//#define INDEX_BASE_DISABLE_DEFINE_USINGS
#include <inba/inba.hpp>

namespace
{
    namespace testtypes
    {
        struct StronglyIncrementable
        {
            StronglyIncrementable& operator++() &;
            StronglyIncrementable operator++(int) &;
        };
        struct WeaklyIncrementable
        {
            WeaklyIncrementable& operator++();
            WeaklyIncrementable operator++(int);
        };

        struct HasValue
        {
            int value();
        };
        struct HasValueBase
        {
            double index_base();
        };
        struct HasValueAndValueBase
        {
            int value();
            double index_base();
        };

        struct HasTypeMember
        {
            typedef int type;
        };

        struct HasValueN
        {
            inba::mpl::NotMyType value();
        };
        struct HasValueBaseN
        {
            inba::mpl::NotMyType index_base();
        };
        struct HasValueAndValueBaseNvNb
        {
            inba::mpl::NotMyType value();
            inba::mpl::NotMyType index_base();
        };
        struct HasValueAndValueBaseNv
        {
            inba::mpl::NotMyType value();
            double index_base();
        };
        struct HasValueAndValueBaseNb
        {
            int value();
            inba::mpl::NotMyType index_base();
        };
    }

    void concept_traits_checks()
    {
        using namespace inba;

        static_assert(std::is_same<First<int, double>::type, int>::value INBA_NTSA);

        static_assert(! has_type_member<int>::value INBA_NTSA);
        static_assert(has_type_member<testtypes::HasTypeMember>::value INBA_NTSA);
        static_assert(has_type_member<std::remove_const<int>>::value INBA_NTSA);

        static_assert(std::is_same<bool_to_type<int, 0>::type, NotMyType>::value INBA_NTSA);
        static_assert(std::is_same<bool_to_type<int, 1>::type, int>::value INBA_NTSA);

        static_assert(! has_type_member<is_same_to_type<int, double>>::value INBA_NTSA);
        static_assert(has_type_member<is_same_to_type<int, int>>::value INBA_NTSA);

        static_assert(cv_ref_removed_is_same<int, int>::value INBA_NTSA);
        static_assert(cv_ref_removed_is_same<int, int&>::value INBA_NTSA);
        static_assert(cv_ref_removed_is_same<int, int&&>::value INBA_NTSA);
        static_assert(cv_ref_removed_is_same<int, int const>::value INBA_NTSA);
        static_assert(cv_ref_removed_is_same<int, int const&>::value INBA_NTSA);
        static_assert(cv_ref_removed_is_same<int, int const&&>::value INBA_NTSA);

        static_assert(std::is_same<get_as_signed_type<unsigned int, NotMyType>::type, int>::value INBA_NTSA);
        static_assert(std::is_same<get_as_signed_type<int, NotMyType>::type, int>::value INBA_NTSA);
        static_assert(std::is_same<get_as_signed_type<int&, NotMyType>::type, int&>::value INBA_NTSA);
        static_assert(std::is_same<get_as_signed_type<void, NotMyType>::type, NotMyType>::value INBA_NTSA);

        static_assert(! has_plus<NotMyType>::value INBA_NTSA);
        static_assert(has_plus<int>::value INBA_NTSA);
        static_assert(! has_minus<NotMyType>::value INBA_NTSA);
        static_assert(has_minus<int>::value INBA_NTSA);
        static_assert(! has_multiply<NotMyType>::value INBA_NTSA);
        static_assert(has_multiply<int>::value INBA_NTSA);
        static_assert(! has_divide<NotMyType>::value INBA_NTSA);
        static_assert(has_divide<int>::value INBA_NTSA);

        static_assert(! has_plus<NotMyType, int>::value INBA_NTSA);
        static_assert(has_plus<int, double>::value INBA_NTSA);
        static_assert(! has_minus<NotMyType, int>::value INBA_NTSA);
        static_assert(has_minus<int, double>::value INBA_NTSA);
        static_assert(! has_multiply<NotMyType, int>::value INBA_NTSA);
        static_assert(has_multiply<int, double>::value INBA_NTSA);
        static_assert(! has_divide<NotMyType, int>::value INBA_NTSA);
        static_assert(has_divide<int, double>::value INBA_NTSA);

        static_assert(! has_increment_prefix<NotMyType>::value INBA_NTSA);
        static_assert(has_increment_prefix<int>::value INBA_NTSA);
        static_assert(! has_increment_postfix<NotMyType>::value INBA_NTSA);
        static_assert(has_increment_postfix<int>::value INBA_NTSA);
        static_assert(! has_decrement_prefix<NotMyType>::value INBA_NTSA);
        static_assert(has_decrement_prefix<int>::value INBA_NTSA);
        static_assert(! has_decrement_postfix<NotMyType>::value INBA_NTSA);
        static_assert(has_decrement_postfix<int>::value INBA_NTSA);

        static_assert(! is_addable<NotMyType>::value INBA_NTSA);
        static_assert(is_addable<int>::value INBA_NTSA);
        static_assert(! is_subtractable<NotMyType>::value INBA_NTSA);
        static_assert(is_subtractable<int>::value INBA_NTSA);
        static_assert(! is_multiplyable<NotMyType>::value INBA_NTSA);
        static_assert(is_multiplyable<int>::value INBA_NTSA);
        static_assert(! is_dividable<NotMyType>::value INBA_NTSA);
        static_assert(is_dividable<int>::value INBA_NTSA);

        static_assert(! is_addable<NotMyType, int>::value INBA_NTSA);
        static_assert(is_addable<int, double>::value INBA_NTSA);
        static_assert(! is_subtractable<NotMyType, int>::value INBA_NTSA);
        static_assert(is_subtractable<int, double>::value INBA_NTSA);
        static_assert(! is_multiplyable<NotMyType, int>::value INBA_NTSA);
        static_assert(is_multiplyable<int, double>::value INBA_NTSA);
        static_assert(! is_dividable<NotMyType, int>::value INBA_NTSA);
        static_assert(is_dividable<int, double>::value INBA_NTSA);

        static_assert(is_weakly_incrementable<testtypes::WeaklyIncrementable>::value INBA_NTSA);
        static_assert(! is_weakly_incrementable<testtypes::StronglyIncrementable>::value INBA_NTSA);
        static_assert(is_strongly_incrementable<int>::value INBA_NTSA);
        static_assert(! is_strongly_incrementable<int const>::value INBA_NTSA);
        static_assert(is_strongly_decrementable<int>::value INBA_NTSA);
        static_assert(! is_strongly_decrementable<int const>::value INBA_NTSA);

        static_assert(is_strongly_in_and_decrementable<int>::value INBA_NTSA);
        static_assert(! is_strongly_in_and_decrementable<int const>::value INBA_NTSA);
        static_assert(is_strongly_in_and_decrementable<float>::value INBA_NTSA);
        static_assert(! is_strongly_in_and_decrementable<void*>::value INBA_NTSA);

        static_assert(! is_add_and_subtractable<NotMyType>::value INBA_NTSA);
        static_assert(is_add_and_subtractable<int const>::value INBA_NTSA);
        static_assert(is_add_and_subtractable<float const&&>::value INBA_NTSA);

        static_assert(! is_add_and_subtractable<NotMyType, int>::value INBA_NTSA);
        static_assert(is_add_and_subtractable<int const, float const>::value INBA_NTSA);
        static_assert(is_add_and_subtractable<int const&, float const&>::value INBA_NTSA);
        static_assert(is_add_and_subtractable<int const&&, float const&&>::value INBA_NTSA);

        static_assert(! can_do_arithmetics<NotMyType, int>::value INBA_NTSA);
        static_assert(can_do_arithmetics<int const, float const>::value INBA_NTSA);
        static_assert(can_do_arithmetics<int const&, float const&>::value INBA_NTSA);
        static_assert(can_do_arithmetics<int const&&, float const&&>::value INBA_NTSA);

        static_assert(std::is_same<diff_type<int const&&, float const&&>::type, float>::value INBA_NTSA);
        static_assert(std::is_same<diff_type<void, int>::type, NotMyType>::value INBA_NTSA);

        static_assert(is_array_index<int>::value INBA_NTSA);
        static_assert(is_array_index<int const&>::value INBA_NTSA);
        static_assert(is_array_index<int const&&>::value INBA_NTSA);
        static_assert(! is_array_index<float>::value INBA_NTSA);
        static_assert(! is_array_index<void*>::value INBA_NTSA);

        static_assert(is_indexed_value_type<int>::value INBA_NTSA);
        static_assert(! is_indexed_value_type<int const>::value INBA_NTSA);
        static_assert(! is_indexed_value_type<int const&>::value INBA_NTSA);
        static_assert(! is_indexed_value_type<float>::value INBA_NTSA);
#if ! defined(INBA_COMPILER_GCC) || (__GNUC__ > 11 || (__GNUC__ == 11 && __GNUC_MINOR__ >= 0))
        // todo: seems like GCC has a problem with void* and the subtraction operator
        static_assert(! is_indexed_value_type<void*>::value INBA_NTSA);
#endif

        static_assert(is_cv_ref_removed_indexed_value_type<int>::value INBA_NTSA);
        static_assert(is_cv_ref_removed_indexed_value_type<int const>::value INBA_NTSA);
        static_assert(is_cv_ref_removed_indexed_value_type<int const&>::value INBA_NTSA);
        static_assert(! is_cv_ref_removed_indexed_value_type<float>::value INBA_NTSA);
#if ! defined(INBA_COMPILER_GCC) || (__GNUC__ > 11 || (__GNUC__ == 11 && __GNUC_MINOR__ >= 0))
        // todo: seems like GCC has a problem with void* and the subtraction operator
        static_assert(! is_cv_ref_removed_indexed_value_type<void*>::value INBA_NTSA);
#endif

        static_assert(is_indexed_value_base_type<int, int>::value INBA_NTSA);
        static_assert(is_indexed_value_base_type<float, int>::value INBA_NTSA);
#if ! defined(INBA_COMPILER_GCC) || (__GNUC__ > 11 || (__GNUC__ == 11 && __GNUC_MINOR__ >= 0))
        // todo: seems like GCC has a problem with void* and the subtraction operator
        static_assert(! is_indexed_value_base_type<void*, int>::value INBA_NTSA);
        static_assert(! is_indexed_value_base_type<float, void*>::value INBA_NTSA);
#endif

        static_assert(! has_Indexed_values<testtypes::HasValue>::value INBA_NTSA);
        static_assert(! has_Indexed_values<testtypes::HasValueBase>::value INBA_NTSA);
        static_assert(! has_Indexed_values<int>::value INBA_NTSA);
        static_assert(has_Indexed_values<testtypes::HasValueAndValueBase>::value INBA_NTSA);

        static_assert(std::is_same<get_Indexed_value_type<testtypes::HasValueAndValueBase, NotMyType>::type, int>::value INBA_NTSA);
        static_assert(
            std::is_same<get_Indexed_value_base_type<testtypes::HasValueAndValueBase, NotMyType>::type, double>::value INBA_NTSA);

        static_assert(! is_pure_Indexed_assignable_value_type<testtypes::HasValueAndValueBase>::value INBA_NTSA);
        static_assert(is_pure_Indexed_assignable_value_type<int>::value INBA_NTSA);
        // todo? static_assert(!is_pure_Indexed_assignable_value_type<float>::value INBA_NTSA);

#if ! defined(INBA_COMPILER_MSVC) || (_MSC_VER > 1900) // todo vs2015
        static_assert(! is_IndexedBase_compatible<testtypes::HasValue>::value INBA_NTSA);
        static_assert(! is_IndexedBase_compatible<testtypes::HasValueBase>::value INBA_NTSA);
        static_assert(! is_IndexedBase_compatible<int>::value INBA_NTSA);
#endif
        static_assert(is_IndexedBase_compatible<testtypes::HasValueAndValueBase>::value INBA_NTSA);

#if ! defined(INBA_COMPILER_MSVC) || (_MSC_VER > 1900) // todo vs2015
        static_assert(! is_IndexedBase_compatible<testtypes::HasValueN>::value INBA_NTSA);
        static_assert(! is_IndexedBase_compatible<testtypes::HasValueBaseN>::value INBA_NTSA);
        static_assert(! is_IndexedBase_compatible<testtypes::HasValueAndValueBaseNv>::value INBA_NTSA);
        static_assert(! is_IndexedBase_compatible<testtypes::HasValueAndValueBaseNb>::value INBA_NTSA);
#endif
        static_assert(! is_IndexedBase_compatible<testtypes::HasValueAndValueBaseNvNb>::value INBA_NTSA);
    }
#ifdef INBA_HAS_CONCEPTS
    void concept_concept_checks()
    {
        using namespace inba;

        static_assert(CvRefRemovedIsSame<int, int>);
        static_assert(CvRefRemovedIsSame<int, int&>);
        static_assert(CvRefRemovedIsSame<int, int&&>);
        static_assert(CvRefRemovedIsSame<int, int const>);
        static_assert(CvRefRemovedIsSame<int, int const&>);
        static_assert(CvRefRemovedIsSame<int, int const&&>);

        static_assert(! Addable<NotMyType>);
        static_assert(Addable<int>);
        static_assert(! Subtractable<NotMyType>);
        static_assert(Subtractable<int>);
        static_assert(! Multiplyable<NotMyType>);
        static_assert(Multiplyable<int>);
        static_assert(! Dividable<NotMyType>);
        static_assert(Dividable<int>);

        static_assert(! Addable<NotMyType, int>);
        static_assert(Addable<int, double>);
        static_assert(! Subtractable<NotMyType, int>);
        static_assert(Subtractable<int, double>);
        static_assert(! Multiplyable<NotMyType, int>);
        static_assert(Multiplyable<int, double>);
        static_assert(! Dividable<NotMyType, int>);
        static_assert(Dividable<int, double>);

        static_assert(WeaklyIncrementable<testtypes::WeaklyIncrementable>);
        static_assert(! WeaklyIncrementable<testtypes::StronglyIncrementable>);
        static_assert(! StronglyIncrementable<testtypes::WeaklyIncrementable>);
        static_assert(StronglyIncrementable<testtypes::StronglyIncrementable>);
        static_assert(StronglyIncrementable<int>);
        static_assert(! StronglyIncrementable<int const>);
        static_assert(StronglyDecrementable<int>);
        static_assert(! StronglyDecrementable<int const>);

        static_assert(StronglyInDecrementable<int>);
        static_assert(! StronglyInDecrementable<int const>);
        static_assert(StronglyInDecrementable<float>);
        static_assert(! StronglyInDecrementable<void*>);

        static_assert(! AddAndSubtractable<NotMyType>);
        static_assert(AddAndSubtractable<int const>);
        static_assert(AddAndSubtractable<float const&&>);

        static_assert(! AddAndSubtractable<NotMyType, int>);
        static_assert(AddAndSubtractable<int const, float const>);
        static_assert(AddAndSubtractable<int const&, float const&>);
        static_assert(AddAndSubtractable<int const&&, float const&&>);

        static_assert(! CanDoArithmetics<NotMyType, int>);
        static_assert(CanDoArithmetics<int const, float const>);
        static_assert(CanDoArithmetics<int const&, float const&>);
        static_assert(CanDoArithmetics<int const&&, float const&&>);

        static_assert(IndexedValueType<int>);
        static_assert(! IndexedValueType<int const>);
        static_assert(! IndexedValueType<int const&>);
        static_assert(! IndexedValueType<float>);
        static_assert(! IndexedValueType<void*>);

        static_assert(CvRefRemovedIndexedValueType<int>);
        static_assert(CvRefRemovedIndexedValueType<int const>);
        static_assert(CvRefRemovedIndexedValueType<int const&>);
        static_assert(! CvRefRemovedIndexedValueType<float>);
        static_assert(! CvRefRemovedIndexedValueType<void*>);

        static_assert(IndexedValueBaseType<int, int>);
        static_assert(IndexedValueBaseType<float, int>);
        static_assert(! IndexedValueBaseType<void*, int>);
        static_assert(! IndexedValueBaseType<float, void*>);

        static_assert(! HasIndexedValues<testtypes::HasValue>);
        static_assert(! HasIndexedValues<testtypes::HasValueBase>);
        static_assert(! HasIndexedValues<int>);
        static_assert(HasIndexedValues<testtypes::HasValueAndValueBase>);

        static_assert(! PureIndexedAssignableValueType<testtypes::HasValueAndValueBase>);
        static_assert(PureIndexedAssignableValueType<int>);
        // todo? static_assert(!PureIndexedAssignableValueType<float>);
    }
#endif
}

TEST_MAIN_START
{
    return EXIT_SUCCESS;
}
TEST_MAIN_END
