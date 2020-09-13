#include "CubeOBBCollsionHandler.h"
#include <algorithm>
#include <math.h>

namespace McEngine
{
namespace Physics
{


CubeOBBCollsionHandler::CubeOBBCollsionHandler(glm::vec3 & p_minimumTranslationVector, 
                                               const glm::vec3 & p_objectACenter, 
                                               const glm::vec3 & p_objectBCenter,
                                               const Meshes::Object* p_objectA,
                                               const Meshes::Object* p_objectB)
    :m_minOverlap(100000.0f),
    m_minimumTranslationVector(p_minimumTranslationVector),
    m_objectCenterA(p_objectACenter),
    m_objectCenterB(p_objectBCenter),
    m_objectA(p_objectA),
    m_objectB(p_objectB)
{
}

std::vector<Meshes::LineSegment> CubeOBBCollsionHandler::getEdges(const Meshes::Collider& p_collider)
{
    std::vector<Meshes::LineSegment> l_result;
    l_result.resize(12);
    std::vector<glm::vec4> l_points = p_collider.m_verticies;
    std::vector<std::vector<int>> l_index = {
        {6,1},
        {6,3},
        {6,4},
        {2,7},
        {2,5},
        {2,0},
        {0,1},
        {0,3},
        {7,1},
        {7,4},
        {4,5},
        {5,3}
    };
    for (int j = 0; j < 12; ++j) {
        l_result.push_back(Meshes::LineSegment(glm::vec3(l_points[l_index[j][0]]), 
                                               glm::vec3(l_points[l_index[j][1]])));
    }
    return l_result;
}

std::vector<Meshes::Plane> CubeOBBCollsionHandler::getFaces(const Meshes::Collider& p_collider,
                                                            const Meshes::Object& p_object)
{
    std::vector<Meshes::Plane> l_result;
    l_result.resize(6);
    auto& l_rigidbody = p_object.m_rigidBody;
    auto& l_pos = p_object.m_transform.m_position;
    glm::vec3 l_size = glm::vec3(l_rigidbody.m_width, l_rigidbody.m_height, l_rigidbody.m_length);
    auto& l_orientation = p_object.m_transform.m_orientation;
    glm::vec3 l_x = glm::vec3(l_orientation[0][0], l_orientation[0][1], l_orientation[0][2]);
    glm::vec3 l_y = glm::vec3(l_orientation[1][0], l_orientation[1][1], l_orientation[1][2]);
    glm::vec3 l_z = glm::vec3(l_orientation[2][0], l_orientation[2][1], l_orientation[2][2]);

    l_result[0] = Meshes::Plane(l_x, glm::dot(l_x, (l_pos + l_x * l_size.x)));
    l_result[1] = Meshes::Plane(l_x * -1.0f, -glm::dot(l_x, (l_pos - l_x * l_size.x)));
    l_result[2] = Meshes::Plane(l_y, glm::dot(l_y, (l_pos + l_y * l_size.y)));
    l_result[3] = Meshes::Plane(l_y * -1.0f, -glm::dot(l_y, (l_pos - l_y * l_size.y)));
    l_result[4] = Meshes::Plane(l_z, glm::dot(l_z, (l_pos + l_z * l_size.z)));
    l_result[5] = Meshes::Plane(l_z * -1.0f, -glm::dot(l_z, (l_pos - l_z * l_size.z)));

    return l_result;
}

bool CubeOBBCollsionHandler::clipToPlane(const Meshes::Plane& p_plane,
                                         const Meshes::LineSegment& p_line,
                                         glm::vec3* p_outPoint)
{
    return false;
}

std::vector<glm::vec3> CubeOBBCollsionHandler::clipEdgesToCube(const std::vector<Meshes::LineSegment>& p_edges,
                                                               const Meshes::Collider& p_collider)
{
    return std::vector<glm::vec3>();
}

float CubeOBBCollsionHandler::findPenetrationDepth(const Meshes::Collider& p_colliderA,
                                                   const Meshes::Collider& p_colliderB,
                                                   const glm::vec3& p_axis,
                                                   bool& p_shouldFlip)
{
    return 0.0f;
}

bool CubeOBBCollsionHandler::checkCollision(const Meshes::Collider & p_coliderA,
                                            const Meshes::Collider & p_coliderB)
{
    std::vector<Meshes::LineSegment> l_colliderAEdges = getEdges(p_coliderA);
    std::vector<Meshes::LineSegment> l_colliderBEdges = getEdges(p_coliderB);
    std::vector<glm::vec3> l_colliderANormals;
    std::vector<glm::vec3> l_colliderBNormals;

    l_colliderANormals.push_back(glm::vec3(p_coliderA.m_normals.x));
    l_colliderANormals.push_back(glm::vec3(p_coliderA.m_normals.y));
    l_colliderANormals.push_back(glm::vec3(p_coliderA.m_normals.z));

    l_colliderBNormals.push_back(glm::vec3(p_coliderB.m_normals.x));
    l_colliderBNormals.push_back(glm::vec3(p_coliderB.m_normals.y));
    l_colliderBNormals.push_back(glm::vec3(p_coliderB.m_normals.z));

    if (not checkCollisionForNormalsAxis(p_coliderA, 
                                         p_coliderB, 
                                         l_colliderANormals,
                                         l_colliderBNormals))
    {
        return false;
    }

    for (const auto& vertexA : l_colliderANormals)
    {
        for (const auto& vertexB : l_colliderBNormals)
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

Meshes::ColMainfold CubeOBBCollsionHandler::getColMainfold() const
{
    return Meshes::ColMainfold();
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