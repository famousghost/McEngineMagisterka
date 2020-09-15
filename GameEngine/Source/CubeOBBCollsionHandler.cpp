#include "CubeOBBCollsionHandler.h"
#include "Geometry3dUtils.h"
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
    const auto& l_rigidbody = p_object.m_rigidBody;
    const auto& l_pos = p_object.m_transform.m_position;
    glm::vec3 l_size = glm::vec3(l_rigidbody.m_width, l_rigidbody.m_height, l_rigidbody.m_length);
    const auto& l_orientation = p_object.m_transform.m_orientation;
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
                                                               const Meshes::Collider& p_collider,
                                                               const Meshes::Object& p_object)
{
    std::vector<glm::vec3> l_result;
    l_result.reserve(p_edges.size());
    glm::vec3 l_intersection;
    std::vector<Meshes::Plane> l_planes = getFaces(p_collider, p_object);

    for (int i = 0; i < l_planes.size(); ++i)
    {
        for (int j = 0; j < p_edges.size(); ++j)
        {
            if (Utils::Geometry3dUtils::clipToPlane(l_planes[i], p_edges[j], &l_intersection))
            {
                if (Utils::Geometry3dUtils::checkIfPointIsOnCubeOBB(p_object, l_intersection))
                {
                    l_result.push_back(l_intersection);
                }
            }
        }
    }

    return l_result;
}

float CubeOBBCollsionHandler::findPenetrationDepth(const Meshes::Object& p_objectA,
                                                   const Meshes::Object& p_objectB,
                                                   const glm::vec3& p_axis,
                                                   bool& p_shouldFlip)
{
    Meshes::Interval l_intervalA = Utils::Geometry3dUtils::getIntervalOBB(p_objectA, p_axis);
    Meshes::Interval l_intervalB = Utils::Geometry3dUtils::getIntervalOBB(p_objectB, p_axis);

    if (!((l_intervalB.m_min <= l_intervalA.m_max) && (l_intervalA.m_min <= l_intervalB.m_max))) {
        return 0.0f;
    }

    float l_lenA = l_intervalA.m_max - l_intervalA.m_min;
    float l_lenB = l_intervalB.m_max - l_intervalB.m_min;


    float l_min = fminf(l_intervalA.m_min, l_intervalB.m_min);
    float l_max = fmaxf(l_intervalA.m_max, l_intervalB.m_max);


    float l_length = l_max - l_min;

    if (p_shouldFlip != 0) {
        p_shouldFlip = (l_intervalB.m_min < l_intervalA.m_min);
    }

    return (l_lenA + l_lenB) - l_length;
}

bool CubeOBBCollsionHandler::checkCollision(const Meshes::Collider & p_coliderA,
                                            const Meshes::Collider & p_coliderB)
{
    m_colMainfold = findCollsionFeatures(*m_objectA, *m_objectB);
    return m_colMainfold.m_isColliding;
}

Meshes::ColMainfold CubeOBBCollsionHandler::findCollsionFeatures(const Meshes::Object & p_objectA, 
                                                                 const Meshes::Object & p_objectB)
{
    Meshes::ColMainfold l_mainfoldResult;

    const auto& l_colliderA = p_objectA.m_colider.at(0);
    const auto& l_colliderB = p_objectB.m_colider.at(0);

    Meshes::ColMainfoldHandler::resetColMainfold(&l_mainfoldResult);
    std::array<glm::vec3, 15> l_test = 
    {
        glm::vec3(p_objectA.m_transform.m_orientation[0][0],
                                   p_objectA.m_transform.m_orientation[0][1],
                                   p_objectA.m_transform.m_orientation[0][2]),

        glm::vec3(p_objectA.m_transform.m_orientation[1][0],
                                   p_objectA.m_transform.m_orientation[1][1],
                                   p_objectA.m_transform.m_orientation[1][2]),

        glm::vec3(p_objectA.m_transform.m_orientation[2][0],
                                   p_objectA.m_transform.m_orientation[2][1],
                                   p_objectA.m_transform.m_orientation[2][2]),
                                 
        glm::vec3(p_objectB.m_transform.m_orientation[0][0],
                                   p_objectB.m_transform.m_orientation[0][1],
                                   p_objectB.m_transform.m_orientation[0][2]),

        glm::vec3(p_objectB.m_transform.m_orientation[1][0],
                                   p_objectB.m_transform.m_orientation[1][1],
                                   p_objectB.m_transform.m_orientation[1][2]),

        glm::vec3(p_objectB.m_transform.m_orientation[2][0],
                                   p_objectB.m_transform.m_orientation[2][1],
                                   p_objectB.m_transform.m_orientation[2][2])
    };

    for (int i = 0; i < 3; ++i) {
        l_test[6 + i * 3 + 0] = glm::cross(l_test[i], l_test[0]);
        l_test[6 + i * 3 + 1] = glm::cross(l_test[i], l_test[1]);
        l_test[6 + i * 3 + 2] = glm::cross(l_test[i], l_test[2]);
    }

    glm::vec3* l_hitNormal = nullptr;
    bool l_shouldFlip;

    for (int i = 0; i < 15; ++i)
    {
        if (glm::length(l_test[i]) * glm::length(l_test[i]) < 0.001f) 
        {
            continue;
        }

        float l_depth = findPenetrationDepth(p_objectA, 
                                             p_objectB, 
                                             l_test[i], 
                                             l_shouldFlip);

        if (l_depth <= 0.0f) {
            return l_mainfoldResult;
        }

        else if (l_depth < l_mainfoldResult.m_depth) 
        {
            if (l_shouldFlip) 
            {
                l_test[i] *= -1.0f;
            }
        }
        l_mainfoldResult.m_depth = l_depth;
        l_hitNormal = &l_test[i];
    }

    if (l_hitNormal == 0)
    {
        return l_mainfoldResult;
    }

    glm::vec3 l_axis = glm::normalize(*l_hitNormal);

    std::vector<glm::vec3> l_clipEdgesA = clipEdgesToCube(getEdges(l_colliderB), l_colliderA, p_objectA);
    std::vector<glm::vec3> l_clipEdgesB = clipEdgesToCube(getEdges(l_colliderA), l_colliderB, p_objectB);
    l_mainfoldResult.m_contacts.reserve(l_clipEdgesA.size() + l_clipEdgesB.size());
    l_mainfoldResult.m_contacts.insert(l_mainfoldResult.m_contacts.end(),
        l_clipEdgesA.begin(), l_clipEdgesA.end());
    l_mainfoldResult.m_contacts.insert(l_mainfoldResult.m_contacts.end(),
        l_clipEdgesB.begin(), l_clipEdgesB.end());


    Meshes::Interval l_interval = Utils::Geometry3dUtils::getIntervalOBB(p_objectA, l_axis);
    float l_distance = (l_interval.m_max - l_interval.m_min) * 0.5f - l_mainfoldResult.m_depth * 0.5f;
    glm::vec3 l_pointOnPlane = p_objectA.m_transform.m_position + l_axis * l_distance;
    for (int i = l_mainfoldResult.m_contacts.size() - 1; i >= 0; --i) 
    {
        glm::vec3 l_contact = l_mainfoldResult.m_contacts[i];
        l_mainfoldResult.m_contacts[i] = l_contact + (l_axis * glm::dot(l_axis, l_pointOnPlane - l_contact));
        for (int j = l_mainfoldResult.m_contacts.size() - 1; j > i; --j) 
        {
            if ((glm::length(l_mainfoldResult.m_contacts[j] - l_mainfoldResult.m_contacts[i]) 
                * glm::length(l_mainfoldResult.m_contacts[j] - l_mainfoldResult.m_contacts[i])) < 0.0001f) 
            {
                l_mainfoldResult.m_contacts.erase(l_mainfoldResult.m_contacts.begin() + j);
                break;
            }
        }
    }

    l_mainfoldResult.m_isColliding = true;
    l_mainfoldResult.m_normal = l_axis;
    return l_mainfoldResult;
}

Meshes::ColMainfold CubeOBBCollsionHandler::getColMainfold() const
{
    return m_colMainfold;
}

}//Physics
}//McEngine