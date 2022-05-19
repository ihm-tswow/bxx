#include <string>

#define TEST(name) void name()

namespace bxx
{
    struct test_case
    {
        char const* m_name;
        void (*test_ptr)();
    };

    struct test_collection
    {
        test_case* m_entries;
        int m_count;
    };

}