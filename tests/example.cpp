#include <cassert>
#include <gympl/outputCompilerInfo.hpp>
#include <gympl/testing.hpp>
#include <sstream>
#include <string>

//#define INDEX_BASE_DISABLE_DEFINE_LITERALS
//#define INDEX_BASE_DISABLE_DEFINE_USINGS
#include <inba/inba.hpp>

#define GYMPL_PRINT_CONFIG
#include <gympl/outputConfiguration.hpp>

namespace
{
    using namespace inba;

    //! [example1]
    struct OldOneBasedApiWithoutIndexedBase
    {
        std::string data_;
        std::string const& rawData()
        {
            return data_;
        }

        OldOneBasedApiWithoutIndexedBase()
          : data_("One based index")
        {}

        int getStartIndex() const
        {
            return 5;
        }
        int getEndIndex() const
        {
            return 10;
        }

        std::string::value_type operator[](int index) const
        {
            return data_[index - 1];
        }
    };
    void example1()
    {
        OldOneBasedApiWithoutIndexedBase obi1;

        std::stringstream ss0;
        for (int i = obi1.getStartIndex(); i < obi1.getEndIndex(); ++i)
            ss0 << obi1.rawData()[i - 1];

        std::stringstream ss1;
        for (int i = obi1.getStartIndex(); i < obi1.getEndIndex(); ++i)
            ss1 << obi1[i];

        assert(ss0.str() == ss1.str());
    }
    //! [example1]
}
namespace
{
    using namespace inba;

    //! [example2]
    struct OneBasedApi
    {
        std::string data_;
        std::string const& rawData()
        {
            return data_;
        }

        OneBasedApi()
          : data_("One based index")
        {}

        Indexed_i1 getStartIndex() const
        {
            return 5_i1;
        }
        Indexed_i1 getEndIndex() const
        {
            return 10_i1;
        }

        std::string::value_type operator[](Indexed_i1 index) const
        {
            return data_[index];
        }
    };
    void example2()
    {
        OneBasedApi obi1;

        std::stringstream ss0;
        for (Indexed_i1 i = obi1.getStartIndex(); i < obi1.getEndIndex(); ++i)
            ss0 << obi1.rawData()[i];

        std::stringstream ss1;
        for (Indexed_i1 i = obi1.getStartIndex(); i < obi1.getEndIndex(); ++i)
            ss1 << obi1[i];

        assert(ss0.str() == ss1.str());
    }
    //! [example2]
}

GYMPL_TEST_MAIN_START
{
    example1();
    example2();
    return EXIT_SUCCESS;
}
GYMPL_TEST_MAIN_END
