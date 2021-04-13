#include "testing.hpp"

#include <cassert>

//#define INDEX_BASE_DISABLE_DEFINE_LITERALS
//#define INDEX_BASE_DISABLE_DEFINE_USINGS
#include <inba/inba.hpp>

namespace
{
    using namespace inba;

    //! [example1]
    void example1()
    {
        assert(2 * 2 == 4);
        assert(2 * 2_i1 == 4_i1);
        assert(2_i1 * 2 == 4_i1);
        assert(2 * 2_i1 == 3); // casting to the base value type means casting to the zero base
        assert(2_i1 * 2 == 3);
        // it's not allowed to multiply (or divide) two indices 2_i1 * 2_i1
    }
    //! [example1]

    //! [example2]
    void example2()
    {
        int data[] = {2, 3};
        int sum = 0;
        for (auto i = 1_i1; i <= Indexed_i1{int(sizeof(data) / sizeof(int))}; ++i)
            sum += data[i] * static_cast<int>(i);

        assert(sum == 3); // 2*0 + 3*1
    }
    //! [example2]

    //! [example3]
    void example3()
    {
        int data[] = {2, 3};
        int sum = 0;
        for (auto i = 1_i1; i <= Indexed_i1{int(sizeof(data) / sizeof(int))}; ++i)
            sum += int(data[i] * i);

        assert(sum == 6); // (2*1)-1 + (3*2)-1
    }
    //! [example3]

    //! [example4]
    void example4()
    {
        int data[] = {2, 3};
        Indexed_i1 sum = 1_i1;
        for (auto i = 1_i1; i <= Indexed_i1{int(sizeof(data) / sizeof(int))}; ++i)
            sum += data[i] * i;

        assert(sum == 9_i1 && sum == 8); // 1 + 2*1 + 3*2
    }
    //! [example4]
}

TEST_MAIN_START
{
    example1();
    example2();
    example3();
    example4();
    return EXIT_SUCCESS;
}
TEST_MAIN_END
