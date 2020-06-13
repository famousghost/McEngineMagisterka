#include "CubeOBBCollsionHandler.h"
#include <algorithm>
#include <math.h>

namespace McEngine
{
namespace Physics
{

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

    auto l_projectedColiderAToAX = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderAEdges[0]);
    auto l_projectedColiderAToAY = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderAEdges[1]);
    auto l_projectedColiderAToAZ = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderAEdges[2]);

    auto l_projectedColiderAToBX = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderBEdges[0]);
    auto l_projectedColiderAToBY = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderBEdges[1]);
    auto l_projectedColiderAToBZ = getProjectedPointsToAxis(p_coliderA.m_verticies, l_colliderBEdges[2]);

    auto l_projectedColiderBToBX = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderBEdges[0]);
    auto l_projectedColiderBToBY = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderBEdges[1]);
    auto l_projectedColiderBToBZ = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderBEdges[2]);

    auto l_projectedColiderBToAX = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderAEdges[0]);
    auto l_projectedColiderBToAY = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderAEdges[1]);
    auto l_projectedColiderBToAZ = getProjectedPointsToAxis(p_coliderB.m_verticies, l_colliderAEdges[2]);

    if (not checkCollisionForNormalAxis(l_projectedColiderAToAX,
        l_projectedColiderAToAY,
        l_projectedColiderAToAZ,
        l_projectedColiderBToAX,
        l_projectedColiderBToAY,
        l_projectedColiderBToAZ))
    {
        return false;
    }

    if (not checkCollisionForNormalAxis(l_projectedColiderAToBX,
        l_projectedColiderAToBY,
        l_projectedColiderAToBZ,
        l_projectedColiderBToBX,
        l_projectedColiderBToBY,
        l_projectedColiderBToBZ))
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

            if (not checkCollisionForAxis(l_projectedColiderA,
                l_projectedColiderB))
            {
                return false;
            }
        }
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
bool CubeOBBCollsionHandler::checkCollisionForAxis(const std::vector<double>& p_projectedColliderA,
    const std::vector<double>& p_projectedColliderB)
{
    auto minA = std::min_element(p_projectedColliderA.begin(), p_projectedColliderA.end());
    auto maxA = std::max_element(p_projectedColliderA.begin(), p_projectedColliderA.end());

    auto minB = std::min_element(p_projectedColliderB.begin(), p_projectedColliderB.end());
    auto maxB = std::max_element(p_projectedColliderB.begin(), p_projectedColliderB.end());

    auto l_longSpan = std::abs(std::max(*maxA, *maxB) - std::min(*minA, *minB));
    auto l_sumSpan = std::abs(*maxA - *minA + *maxB - *minB);
    return l_longSpan < l_sumSpan;
}

bool CubeOBBCollsionHandler::checkCollisionForNormalAxis(const std::vector<double>& p_projectedColliderAX,
    const std::vector<double>& p_projectedColliderAY,
    const std::vector<double>& p_projectedColliderAZ,
    const std::vector<double>& p_projectedColliderBX,
    const std::vector<double>& p_projectedColliderBY,
    const std::vector<double>& p_projectedColliderBZ)
{
    auto minAX = std::min_element(p_projectedColliderAX.begin(), p_projectedColliderAX.end());
    auto minAY = std::min_element(p_projectedColliderAY.begin(), p_projectedColliderAY.end());
    auto minAZ = std::max_element(p_projectedColliderAZ.begin(), p_projectedColliderAZ.end());

    auto maxAX = std::max_element(p_projectedColliderAX.begin(), p_projectedColliderAX.end());
    auto maxAY = std::max_element(p_projectedColliderAY.begin(), p_projectedColliderAY.end());
    auto maxAZ = std::min_element(p_projectedColliderAZ.begin(), p_projectedColliderAZ.end());

    auto minBX = std::min_element(p_projectedColliderBX.begin(), p_projectedColliderBX.end());
    auto minBY = std::min_element(p_projectedColliderBY.begin(), p_projectedColliderBY.end());
    auto minBZ = std::max_element(p_projectedColliderBZ.begin(), p_projectedColliderBZ.end());

    auto maxBX = std::max_element(p_projectedColliderBX.begin(), p_projectedColliderBX.end());
    auto maxBY = std::max_element(p_projectedColliderBY.begin(), p_projectedColliderBY.end());
    auto maxBZ = std::min_element(p_projectedColliderBZ.begin(), p_projectedColliderBZ.end());

    auto l_longSpan = std::abs(std::max(*maxAX, *maxBX) - std::min(*minAX, *minBX));
    auto l_sumSpan = std::abs(*maxAX - *minAX + *maxBX - *minBX);

    if (l_longSpan >= l_sumSpan)
    {
        return false;
    }

    l_longSpan = std::abs(std::max(*maxAY, *maxBY) - std::min(*minAY, *minBY));
    l_sumSpan = std::abs(*maxAY - *minAY + *maxBY - *minBY);

    if (l_longSpan >= l_sumSpan)
    {
        return false;
    }

    l_longSpan = std::abs(std::min(*maxAZ, *maxBZ) - std::max(*minAZ, *minBZ));
    l_sumSpan = std::abs(*minAZ - *maxAZ + *minBZ - *maxBZ);

    if (l_longSpan >= l_sumSpan)
    {
        return false;
    }

    return true;
}
}//Physics
}//McEngine