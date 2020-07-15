#include "CubeOBBCollsionHandler.h"
#include <algorithm>
#include <math.h>

namespace McEngine
{
namespace Physics
{


CubeOBBCollsionHandler::CubeOBBCollsionHandler(glm::vec3 & p_minimumTranslationVector, 
                                               const glm::vec3 & p_objectACenter, 
                                               const glm::vec3 & p_objectBCenter)
    :m_minOverlap(100000.0f),
    m_minimumTranslationVector(p_minimumTranslationVector),
    m_objectCenterA(p_objectACenter),
    m_objectCenterB(p_objectBCenter)
{
}

bool CubeOBBCollsionHandler::checkCollision(const Meshes::Collider & p_coliderA,
                                            const Meshes::Collider & p_coliderB)
{
    std::vector<glm::vec3> l_colliderAEdges;
    std::vector<glm::vec3> l_colliderBEdges;

    l_colliderAEdges.push_back(glm::vec3(p_coliderA.m_normals.x));
    l_colliderAEdges.push_back(glm::vec3(p_coliderA.m_normals.y));
    l_colliderAEdges.push_back(glm::vec3(p_coliderA.m_normals.z));

    l_colliderBEdges.push_back(glm::vec3(p_coliderB.m_normals.x));
    l_colliderBEdges.push_back(glm::vec3(p_coliderB.m_normals.y));
    l_colliderBEdges.push_back(glm::vec3(p_coliderB.m_normals.z));

    if (not checkCollisionForNormalsAxis(p_coliderA, 
                                         p_coliderB, 
                                         l_colliderAEdges,
                                         l_colliderBEdges))
    {
        return false;
    }

    for (const auto& vertexA : l_colliderAEdges)
    {
        for (const auto& vertexB : l_colliderBEdges)
        {
            auto l_normal = glm::cross(vertexA, vertexB);

            if (l_normal == glm::vec3())
            {
                continue;
            }

            auto l_projectedColiderA = getProjectedPointsToAxis(p_coliderA.m_verticies, l_normal);
            auto l_projectedColiderB = getProjectedPointsToAxis(p_coliderB.m_verticies, l_normal);

            auto l_overlap = checkCollisionForAxis(l_projectedColiderA,
                                                   l_projectedColiderB);

            if (not l_overlap)
            {
                return false;
            }
            else
            {
                if (m_minOverlap > l_overlap)
                {
                    m_minOverlap = l_overlap;
                    m_minimumTranslationVector = l_normal;
                }
            }
        }
    }

    if (glm::dot(m_objectCenterA - m_objectCenterB, m_minimumTranslationVector) < 0)
    {
        m_minimumTranslationVector = -m_minimumTranslationVector;
    }

    return true;
}

std::vector<double> CubeOBBCollsionHandler::getProjectedPointsToAxis(const std::vector<glm::vec4>& p_listOfVec,
    const glm::vec3& p_normals)
{
    std::vector<double> l_result;
    for (const auto& vec : p_listOfVec)
    {
        auto value = glm::dot(glm::vec3(vec), glm::normalize(p_normals));

        l_result.push_back(value);
    }
    return l_result;
}

double CubeOBBCollsionHandler::checkCollisionForAxis(const std::vector<double>& p_projectedColliderA,
                                                     const std::vector<double>& p_projectedColliderB)
{
    auto minA = std::min_element(p_projectedColliderA.begin(), p_projectedColliderA.end());
    auto maxA = std::max_element(p_projectedColliderA.begin(), p_projectedColliderA.end());

    auto minB = std::min_element(p_projectedColliderB.begin(), p_projectedColliderB.end());
    auto maxB = std::max_element(p_projectedColliderB.begin(), p_projectedColliderB.end());

    auto l_longSpan = std::abs(std::max(*maxA, *maxB) - std::min(*minA, *minB));
    auto l_sumSpan = std::abs(*maxA - *minA + *maxB - *minB);
    return (l_longSpan < l_sumSpan) ? std::abs(l_longSpan - l_sumSpan) : 0.0;
}

bool CubeOBBCollsionHandler::checkCollisionForNormalsAxis(const Meshes::Collider & p_coliderA,
                                                          const Meshes::Collider & p_coliderB,
                                                          const std::vector<glm::vec3>& p_colliderAEdges,
                                                          const std::vector<glm::vec3>& p_colliderBEdges)
{
    for(std::size_t i = 0; i < 3; ++i)
    {
        auto l_overlap = checkCollisionForNormalAxis(getProjectedPointsToAxis(p_coliderA.m_verticies, p_colliderAEdges[i]),
                                                     getProjectedPointsToAxis(p_coliderB.m_verticies, p_colliderAEdges[i]));
        if (not l_overlap)
        {
            return false;
        }
        else
        {
            if (m_minOverlap > l_overlap)
            {
                m_minOverlap = l_overlap;
                m_minimumTranslationVector = p_colliderAEdges[i];
            }
        }
    }
    for (std::size_t i = 0; i < 3; ++i)
    {
        auto l_overlap = checkCollisionForNormalAxis(getProjectedPointsToAxis(p_coliderA.m_verticies, p_colliderBEdges[i]),
                                                 getProjectedPointsToAxis(p_coliderB.m_verticies, p_colliderBEdges[i]));
        if (not l_overlap)
        {
            return false;
        }
        else
        {
            if (m_minOverlap > l_overlap)
            {
                m_minOverlap = l_overlap;
                m_minimumTranslationVector = p_colliderBEdges[i];
            }
        }
    }
    return true;
}

double CubeOBBCollsionHandler::checkCollisionForNormalAxis(const std::vector<double>& p_projectedColliderA, 
                                                           const std::vector<double>& p_projectedColliderB)
{
    auto minA = std::min_element(p_projectedColliderA.begin(), p_projectedColliderA.end());
    auto maxA = std::max_element(p_projectedColliderA.begin(), p_projectedColliderA.end());

    auto minB = std::min_element(p_projectedColliderB.begin(), p_projectedColliderB.end());
    auto maxB = std::max_element(p_projectedColliderB.begin(), p_projectedColliderB.end());

    auto l_longSpan = std::abs(std::max(*maxA, *maxB) - std::min(*minA, *minB));
    auto l_sumSpan = std::abs(*maxA - *minA + *maxB - *minB);

    return (l_longSpan >= l_sumSpan) ? 0.0 : std::abs(l_longSpan - l_sumSpan);
}
}//Physics
}//McEngine