#pragma once
#include <vector>
#include <limits>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Meshes
{
struct ColMainfold
{
    bool m_isColliding;
    glm::vec3 m_normal;
    float m_depth;
    std::vector<glm::vec3> m_contacts;
};

struct ColMainfoldHandler
{
    static void resetColMainfold(ColMainfold* p_colMainfold)
    {
        if (p_colMainfold)
        {
            p_colMainfold->m_isColliding = false;
            p_colMainfold->m_normal = glm::vec3(0.0f, 0.0f, -1.0f);
            p_colMainfold->m_depth = std::numeric_limits<float>::max();
            p_colMainfold->m_contacts.clear();
        }
    }
};

}//Physics
}//McEngine

