#include "SphereVOBBCollisionHandler.h"
#include <algorithm>

namespace McEngine
{
namespace Physics
{

SphereVOBBCollisionHandler::SphereVOBBCollisionHandler(const glm::vec3 & p_objectCenterA, const glm::vec3 & p_objectCenterB)
    :m_objectCenterA(p_objectCenterA), m_objectCenterB(p_objectCenterB)
{
}

bool SphereVOBBCollisionHandler::checkCollision(const Meshes::Collider & p_coliderA, const Meshes::Collider & p_coliderB)
{
    glm::vec3 l_closestPointOnOBBCube;
    float l_distance;
    float l_radius;
    if (p_coliderA.m_colliderType == Meshes::ColliderType::CUBE_OBB)
    {
        glm::mat4 l_inverseTransformPoint = glm::inverse(p_coliderA.m_modelMatrix);
        glm::vec4 l_localSphereCenter = l_inverseTransformPoint * glm::vec4((m_objectCenterB + p_coliderB.m_transform.m_position), 1.0f);

        glm::vec4 l_localXSectionMin = l_inverseTransformPoint * p_coliderA.m_xSection.min;
        glm::vec4 l_localXSectionMax = l_inverseTransformPoint * p_coliderA.m_xSection.max;

        glm::vec4 l_localYSectionMin = l_inverseTransformPoint * p_coliderA.m_ySection.min;
        glm::vec4 l_localYSectionMax = l_inverseTransformPoint * p_coliderA.m_ySection.max;

        glm::vec4 l_localZSectionMin = l_inverseTransformPoint * p_coliderA.m_zSection.min;
        glm::vec4 l_localZSectionMax = l_inverseTransformPoint * p_coliderA.m_zSection.max;
        l_closestPointOnOBBCube =
            glm::vec3(
                std::max(l_localXSectionMin.x, std::min(l_localSphereCenter.x, l_localXSectionMax.x)),
                std::max(l_localYSectionMin.y, std::min(l_localSphereCenter.y, l_localYSectionMax.y)),
                std::min(l_localZSectionMin.z, std::max(l_localSphereCenter.z, l_localZSectionMax.z)));
        l_distance = glm::distance(l_closestPointOnOBBCube, glm::vec3(l_localSphereCenter));
        l_radius = p_coliderB.m_radius;
    }
    else
    {
        glm::mat4 l_inverseTransformPoint = glm::inverse(p_coliderB.m_modelMatrix);
        glm::vec4 l_localSphereCenter = l_inverseTransformPoint * glm::vec4((m_objectCenterA + p_coliderA.m_transform.m_position), 1.0f);

        glm::vec4 l_localXSectionMin = l_inverseTransformPoint * p_coliderB.m_xSection.min;
        glm::vec4 l_localXSectionMax = l_inverseTransformPoint * p_coliderB.m_xSection.max;

        glm::vec4 l_localYSectionMin = l_inverseTransformPoint * p_coliderB.m_ySection.min;
        glm::vec4 l_localYSectionMax = l_inverseTransformPoint * p_coliderB.m_ySection.max;

        glm::vec4 l_localZSectionMin = l_inverseTransformPoint * p_coliderB.m_zSection.min;
        glm::vec4 l_localZSectionMax = l_inverseTransformPoint * p_coliderB.m_zSection.max;
        l_closestPointOnOBBCube =
            glm::vec3(
                std::max(l_localXSectionMin.x, std::min(l_localSphereCenter.x, l_localXSectionMax.x)),
                std::max(l_localYSectionMin.y, std::min(l_localSphereCenter.y, l_localYSectionMax.y)),
                std::min(l_localZSectionMin.z, std::max(l_localSphereCenter.z, l_localZSectionMax.z)));
        l_distance = glm::distance(l_closestPointOnOBBCube, glm::vec3(l_localSphereCenter));
        l_radius = p_coliderA.m_radius;
    }

    return l_distance <= l_radius;
}

}//Physics
}//McEngine