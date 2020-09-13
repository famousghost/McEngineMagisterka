#pragma once
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Physics 
{

struct RaycastResult
{
    glm::vec3 m_point;
    glm::vec3 m_normal;
    float m_t;
    bool m_hit;
};

struct RaycastResultHandler
{
    static void raycastResultReset(RaycastResult* p_raycastResult)
    {
        if(p_raycastResult)
        {
            p_raycastResult->m_point = glm::vec3();
            p_raycastResult->m_normal = glm::vec3();
            p_raycastResult->m_hit = false;
            p_raycastResult->m_t = -1.0f;
        }
    }
};

}
}

