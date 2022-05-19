#include <string>

#define BXX_TEST(name) void __test_##name()

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