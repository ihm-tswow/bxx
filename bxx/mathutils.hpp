#include <ostream>

namespace bxx
{
    struct vec3
    {
        vec3(float x, float y, float z);
        float x;
        float y;
        float z;
    };

    struct quaternion
    {
        quaternion(float w, float x, float y, float z);
        float w;
        float x;
        float y;
        float z;
    };

    bool operator==(const bxx::vec3& lhs, const bxx::vec3& rhs);
    bool operator==(const bxx::quaternion& lhs, const bxx::quaternion& rhs);
    bool operator!=(const bxx::vec3& lhs, const bxx::vec3& rhs);
    bool operator!=(const bxx::quaternion& lhs, const bxx::quaternion& rhs);
    std::ostream& operator<<(std::ostream& os, const vec3& dt);
    std::ostream& operator<<(std::ostream& os, const quaternion& dt);
}