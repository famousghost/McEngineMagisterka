#include "Geometry3dUtils.h"
#include <math.h>

namespace McEngine
{
namespace Utils
{ 

float Geometry3dUtils::cmp(float x, float y)
{
    return std::fabsf(x - y) 
           <= FLT_EPSILON * std::fmaxf(1.0f, std::fmaxf(std::fabsf(x), std::fabsf(y)));
}

Ray Geometry3dUtils::fromPoints(const glm::vec3 & p_from, const glm::vec3 & p_to)
{
    return Ray(p_from, glm::normalize(p_to - p_from));
}

bool Geometry3dUtils::pointOnRay(const glm::vec3& p_point, const Ray& p_ray)
{
    if (p_point == p_ray.m_origin)
    {
        return true;
    }
    auto l_normalFromPointToOrigin = glm::normalize(p_point - p_ray.m_origin);

    return glm::dot(l_normalFromPointToOrigin, p_ray.m_direction) == 1.0f;

}

glm::vec3 Geometry3dUtils::closestPoint(const Ray& p_ray, const glm::vec3& p_point)
{
    auto l_t = glm::dot(p_point - p_ray.m_origin, p_ray.m_direction);

    l_t /= (glm::length(p_ray.m_direction) * glm::length(p_ray.m_direction));

    l_t = std::fmax(l_t, 0.0f);

    return p_ray.m_origin + p_ray.m_direction * l_t;
}

bool Geometry3dUtils::raycastSphere(const Meshes::Object& p_object, 
                                    const Meshes::Collider& p_collider, 
                                    const Ray& p_ray,
                                    Physics::RaycastResult* p_raycastResult)
{
    auto l_vecFromOriginToSphereCenter = p_object.m_transform.m_position - p_ray.m_origin;

    float l_magnitudeOfVecSquare = glm::length(l_vecFromOriginToSphereCenter) * glm::length(l_vecFromOriginToSphereCenter);
    float l_radiusSquare = p_collider.m_radius * p_collider.m_radius;

    float l_castVecToRay = glm::dot(l_vecFromOriginToSphereCenter, p_ray.m_direction);

    auto l_bSq = l_magnitudeOfVecSquare - l_castVecToRay * l_castVecToRay;
    auto l_f = std::sqrt(l_radiusSquare - l_bSq);
    float l_t = l_castVecToRay - l_f;
    if (l_radiusSquare - l_bSq < 0.0f)
    {
        return false;
    }
    if (l_magnitudeOfVecSquare < l_radiusSquare)
    {
        l_t = l_castVecToRay + l_f;
    }
    if (p_raycastResult)
    {
        p_raycastResult->m_t = l_t;
        p_raycastResult->m_hit = true;
        p_raycastResult->m_point = p_ray.m_origin + p_ray.m_direction * l_t;
        p_raycastResult->m_normal = 
            glm::normalize(p_raycastResult->m_point - p_object.m_transform.m_position);
    }

    return true;
}

bool Geometry3dUtils::findClosestPointOnCubeAABB(glm::vec3 p_max,
                                             glm::vec3 p_min, 
                                             const Ray& p_ray,
                                             Physics::RaycastResult* p_raycastResult)
{
    float l_maxMin = std::fmax(std::fmax(std::fmin(p_min.x, p_max.x), std::fmin(p_min.y, p_max.y)), std::fmin(p_min.z, p_max.z));
    float l_minMax = std::fmin(std::fmin(std::fmax(p_min.x, p_max.x), std::fmax(p_min.y, p_max.y)), std::fmax(p_min.z, p_max.z));
    float l_t[] = {p_min.x, p_max.x, p_min.y, p_max.y, p_min.z, p_max.z};

    if (l_minMax < 0.0f)
    {
        return false;
    }
    if (l_maxMin > l_minMax)
    {
        return false;
    }
    float l_tResult = l_maxMin;
    if (l_maxMin < 0.0f)
    {
        l_tResult = l_minMax;
    }
    if (p_raycastResult)
    {
        p_raycastResult->m_point = p_ray.m_origin + p_ray.m_direction * l_tResult;
        p_raycastResult->m_hit = true;
        p_raycastResult->m_t = l_tResult;
        glm::vec3 l_normals[] = 
        {
            glm::vec3(-1, 0, 0), 
            glm::vec3(1, 0, 0),
            glm::vec3(0, -1, 0), 
            glm::vec3(0, 1, 0),
            glm::vec3(0, 0, -1),
            glm::vec3(0, 0, 1)
        };
        for (int i = 0; i < 6; ++i) 
        {
            if (cmp(l_tResult, l_t[i]))
            {
                p_raycastResult->m_normal = l_normals[i];
            }
        }
    }

    return true;
}

bool Geometry3dUtils::findClosestPointOnCubeOBB(const glm::vec3& p_orientationX,
                                                const glm::vec3& p_orientationY,
                                                const glm::vec3& p_orientationZ,
                                                const std::array<float, MAX_MIN_VALUES_SIZE>& p_t,
                                                const Ray & p_ray,
                                                Physics::RaycastResult * p_raycastResult)
{
    float l_maxMin = std::fmax(std::fmax(std::fmin(p_t[0], p_t[1]), std::fmin(p_t[2], p_t[3])), std::fmin(p_t[4], p_t[5]));
    float l_minMax = std::fmin(std::fmin(std::fmax(p_t[0], p_t[1]), std::fmax(p_t[2], p_t[3])), std::fmax(p_t[4], p_t[5]));

    if (l_minMax < 0.0f)
    {
        return false;
    }
    if (l_maxMin > l_minMax)
    {
        return false;
    }
    float l_tResult = l_maxMin;
    if (l_maxMin < 0.0f)
    {
        l_tResult = l_minMax;
    }
    if (p_raycastResult)
    {
        p_raycastResult->m_point = p_ray.m_origin + p_ray.m_direction * l_tResult;
        p_raycastResult->m_hit = true;
        p_raycastResult->m_t = l_tResult;
        glm::vec3 l_normals[] =
        {
            p_orientationX,
            p_orientationX * -1.0f,
            p_orientationY,
            p_orientationY * -1.0f,
            p_orientationZ,
            p_orientationZ * -1.0f
        };
        for (int i = 0; i < 6; ++i)
        {
            if (cmp(l_tResult, p_t[i]))
            {
                p_raycastResult->m_normal = glm::normalize(l_normals[i]);
            }
        }
    }

    return true;
}

bool Geometry3dUtils::raycastAABB(const Meshes::Object & p_object, 
                                   const Meshes::Collider& p_collider, 
                                   const Ray & p_ray,
                                   Physics::RaycastResult* p_raycastResult)
{
    auto l_min = p_collider.m_minVertex;
    auto l_max = p_collider.m_maxVertex;
    float txmin = 0.0f;
    float txmax = 0.0f;
    float tymin = 0.0f;
    float tymax = 0.0f;
    float tzmin = 0.0f;
    float tzmax = 0.0f;
    if (p_ray.m_direction.x != 0.0f)
    {
        txmin = (l_min.x - p_ray.m_origin.x) / p_ray.m_direction.x;
        txmax = (l_max.x - p_ray.m_origin.x) / p_ray.m_direction.x;
    }
    if (p_ray.m_direction.y != 0.0f)
    {
        tymin = (l_min.y - p_ray.m_origin.y) / p_ray.m_direction.y;
        tymax = (l_max.y - p_ray.m_origin.y) / p_ray.m_direction.y;
    }
    if (p_ray.m_direction.z != 0.0f)
    {
        tzmin = (l_min.z - p_ray.m_origin.z) / p_ray.m_direction.z;
        tzmax = (l_max.z - p_ray.m_origin.z) / p_ray.m_direction.z;
    }

    return findClosestPointOnCubeAABB(glm::vec3(txmax, tymax, tzmax), 
                                      glm::vec3(txmin, tymin, tzmin),
                                      p_ray,
                                      p_raycastResult);
}

bool Geometry3dUtils::raycastOBB(const Meshes::Object & p_object,
                                  const Meshes::Collider& p_collider,
                                  const Ray & p_ray,
                                  Physics::RaycastResult* p_raycastResult)
{
    auto& l_collider = p_object.m_colider.at(0);
    auto& l_objectProperties = p_object.m_rigidBody;
    glm::vec3 l_size = glm::vec3(l_collider.m_width + l_objectProperties.m_width,
                                 l_collider.m_height + l_objectProperties.m_height,
                                 l_collider.m_length + l_objectProperties.m_length);

    auto& l_orientation = l_collider.m_transform.m_orientation;
    glm::vec3 l_x = glm::vec3(l_orientation[0][0], l_orientation[0][1], l_orientation[0][2]);
    glm::vec3 l_y = glm::vec3(l_orientation[1][0], l_orientation[1][1], l_orientation[1][2]);
    glm::vec3 l_z = glm::vec3(l_orientation[2][0], l_orientation[2][1], l_orientation[2][2]);
    
    glm::vec3 l_p = (p_object.m_transform.m_position + l_collider.m_transform.m_position) - p_ray.m_origin;

    glm::vec3 l_projectionOfDirectionToEachAxis = glm::vec3(glm::dot(l_x, p_ray.m_direction),
                                                            glm::dot(l_y, p_ray.m_direction),
                                                            glm::dot(l_z, p_ray.m_direction));

    glm::vec3 l_projectionOfPToEachAxis = glm::vec3(glm::dot(l_x, l_p),
                                                    glm::dot(l_y, l_p),
                                                    glm::dot(l_z, l_p));


    std::array<float, MAX_MIN_VALUES_SIZE> l_t {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    for (int i = 0; i < 3; ++i)
    {
        if (cmp(l_projectionOfDirectionToEachAxis[i], 0))
        {
            if (-l_projectionOfPToEachAxis[i] - l_size[i] > 0 
                || -l_projectionOfPToEachAxis[i] + l_size[i] < 0) 
            {
                return false;
            }
            l_projectionOfDirectionToEachAxis[i] = 0.00001f;
        }
        l_t[i * 2 + 0] = 
            (l_projectionOfPToEachAxis[i] + l_size[i]) / l_projectionOfDirectionToEachAxis[i]; // min
        l_t[i * 2 + 1] = 
            (l_projectionOfPToEachAxis[i] - l_size[i]) / l_projectionOfDirectionToEachAxis[i]; // max
    }

    return findClosestPointOnCubeOBB(l_x, 
                                     l_y,
                                     l_z,
                                     l_t,
                                     p_ray,
                                     p_raycastResult);
}

bool Geometry3dUtils::raycast(const Meshes::Object & p_object, 
                               const Ray & p_ray, 
                               Physics::RaycastResult* p_raycastResult)
{
    Physics::RaycastResultHandler::raycastResultReset(p_raycastResult);
    auto& l_collider = p_object.m_colider.at(0);
    if (l_collider.m_colliderType == Meshes::ColliderType::SPHERE)
    {
        return raycastSphere(p_object, l_collider, p_ray, p_raycastResult);
    }
    if(l_collider.m_colliderType == Meshes::ColliderType::CUBE_AABB)
    {
        return raycastAABB(p_object, l_collider, p_ray, p_raycastResult);
    }
    if (l_collider.m_colliderType == Meshes::ColliderType::CUBE_OBB)
    {
        return raycastOBB(p_object, l_collider, p_ray, p_raycastResult);
    }
    return false;
}

bool Geometry3dUtils::clipToPlane(const Meshes::Plane& p_plane,
                                  const Meshes::LineSegment& p_line,
                                  glm::vec3* p_outPoint)
{
    glm::vec3 l_line = p_line.m_endPoint - p_line.m_startPoint;
    float l_nAB = glm::dot(p_plane.m_normal, l_line);

    if (cmp(l_nAB, 0)) 
    {
        return false;
    }

    float l_nA = glm::dot(p_plane.m_normal, p_line.m_startPoint);
    float l_t = (p_plane.m_distance - l_nA) / l_nAB;

    if (l_t >= 0.0f and l_t <= 1.0f)
    {
        if (p_outPoint != 0)
        {
            *p_outPoint = p_line.m_startPoint + l_line * l_t;
        }
        return true;
    }
    return false;
}



bool Geometry3dUtils::checkIfPointIsOnCubeOBB(const Meshes::Object& p_object,
                                              const glm::vec3& p_point)
{
    glm::vec3 l_dir = p_point - p_object.m_transform.m_position;

    glm::vec3 l_size = glm::vec3(p_object.m_rigidBody.m_width,
                                 p_object.m_rigidBody.m_height,
                                 p_object.m_rigidBody.m_length);

    for (int i = 0; i < 3; ++i)
    {
        glm::vec3 l_axis =
            glm::vec3(p_object.m_transform.m_orientation[i][0],
                      p_object.m_transform.m_orientation[i][1],
                      p_object.m_transform.m_orientation[i][2]);

        float l_distance = glm::dot(l_dir, l_axis);

        if (l_distance > l_size[i] or l_distance < -l_size[i])
        {
            return false;
        }
    }

    return true;
}

glm::vec3 Geometry3dUtils::findClosestPointOnCubeOBB(const Meshes::Object& p_object,
                                                     const glm::vec3& p_point)
{
    glm::vec3 l_result = p_object.m_transform.m_position;

    glm::vec3 l_dir = p_point - p_object.m_transform.m_position;

    glm::vec3 l_size = glm::vec3(p_object.m_colider.at(0).m_width,
                                 p_object.m_colider.at(0).m_height,
                                 p_object.m_colider.at(0).m_length);

    for (int i = 0; i < 3; ++i)
    {
        glm::vec3 l_axis =
            glm::vec3(p_object.m_colider.at(0).m_transform.m_orientation[i][0],
                      p_object.m_colider.at(0).m_transform.m_orientation[i][1],
                      p_object.m_colider.at(0).m_transform.m_orientation[i][2]);

        float l_distance = glm::dot(l_dir, l_axis);

        if (l_distance > l_size[i])
        {
            l_distance = l_size[i];
        }
        if (l_distance < -l_size[i])
        {
            l_distance = -l_size[i];
        }

        l_result += l_axis * l_distance;
    }

    return l_result;
}

Meshes::Interval Geometry3dUtils::getIntervalABB(const Meshes::Object& p_object,
                                                 const glm::vec3& p_axis)
{
    auto& l_collider = p_object.m_colider.at(0);
    glm::vec3 l_min = glm::vec3(l_collider.m_minVertex.x, 
                                l_collider.m_minVertex.y, 
                                l_collider.m_minVertex.z);
    glm::vec3 l_max = glm::vec3(l_collider.m_maxVertex.x, 
                                l_collider.m_maxVertex.y, 
                                l_collider.m_maxVertex.z);

    std::vector<glm::vec3> l_verticies =
    {
        glm::vec3(l_min.x, l_max.y, l_max.z),
        glm::vec3(l_min.x, l_max.y, l_min.z),
        glm::vec3(l_min.x, l_min.y, l_max.z),
        glm::vec3(l_min.x, l_min.y, l_min.z),
        glm::vec3(l_max.x, l_max.y, l_max.z),
        glm::vec3(l_max.x, l_max.y, l_min.z),
        glm::vec3(l_max.x, l_min.y, l_max.z),
        glm::vec3(l_max.x, l_min.y, l_min.z)
    };

    Meshes::Interval l_result;
    l_result.m_min = l_result.m_max = glm::dot(p_axis, l_verticies[0]);
    for (int i = 1; i < 8; ++i) {
        float l_projection = glm::dot(p_axis, l_verticies[i]);
        l_result.m_min = (l_projection < l_result.m_min) ?
            l_projection : l_result.m_min;
        l_result.m_max = (l_projection > l_result.m_max) ?
            l_projection : l_result.m_max;
    }
    return l_result;
}

Meshes::Interval Geometry3dUtils::getIntervalOBB(const Meshes::Object & p_object, 
                                                 const glm::vec3 & p_axis)
{
    std::vector<glm::vec4> l_verticies = p_object.m_colider.at(0).m_verticies;

    Meshes::Interval l_result;
    l_result.m_min = l_result.m_max = glm::dot(p_axis, glm::vec3(l_verticies[0]));
    for (int i = 1; i < 8; ++i) {
        float l_projection = glm::dot(p_axis, glm::vec3(l_verticies[i]));
        l_result.m_min = (l_projection < l_result.m_min) ? l_projection : l_result.m_min;
        l_result.m_max = (l_projection > l_result.m_max) ? l_projection : l_result.m_max;
    }

    return l_result;
}

void Geometry3dUtils::applyImpulse(Meshes::Object & p_objectA, 
                                   Meshes::Object & p_objectB, 
                                   Meshes::ColMainfold & p_mainfold, 
                                   int p_c)
{
    float l_invMassA = p_objectA.m_rigidBody.m_massProperties.m_inverseMass;
    float l_invMassB = p_objectB.m_rigidBody.m_massProperties.m_inverseMass;
    float l_invMassSum = l_invMassA + l_invMassB;

    if (not l_invMassSum) 
    { 
        return; 
    }

    glm::vec3 l_rA = p_mainfold.m_contacts[p_c] - p_objectA.m_transform.m_position;
    glm::vec3 l_rB = p_mainfold.m_contacts[p_c] - p_objectB.m_transform.m_position;

    glm::mat3 l_interiaTensorA = p_objectA.m_rigidBody.m_invBodyTensorOfInteria;
    glm::mat3 l_interiaTensorB = p_objectB.m_rigidBody.m_invBodyTensorOfInteria;

    glm::vec3 l_relativeVel = 
        (p_objectB.m_rigidBody.m_velocity + glm::cross(p_objectB.m_rigidBody.m_angularVelocity, l_rB))
        - (p_objectA.m_rigidBody.m_velocity + glm::cross(p_objectA.m_rigidBody.m_angularVelocity, l_rA));

    glm::vec3 l_relativeNorm = glm::normalize(p_mainfold.m_normal);

    if (glm::dot(l_relativeVel, l_relativeNorm) > 0.0f) {
        return;
    }

    float l_e = fminf(p_objectA.m_rigidBody.m_materialProperties.m_restitution,
                      p_objectB.m_rigidBody.m_materialProperties.m_restitution);
    float l_numerator = (-(1.0f + l_e) * glm::dot(l_relativeVel, l_relativeNorm));

    float l_d = l_invMassSum;

    glm::vec3 l_dvecA = glm::cross(glm::cross(l_rA, l_relativeNorm) * l_interiaTensorA, l_rA);
    glm::vec3 l_dvecB = glm::cross(glm::cross(l_rB, l_relativeNorm) * l_interiaTensorB, l_rB);

    float l_denominator = l_d + glm::dot(l_relativeNorm, l_dvecA + l_dvecB);

    float l_j = (l_denominator == 0.0f) ? 0.0f : l_numerator / l_denominator;


    if (p_mainfold.m_contacts.size() > 0.0f && l_j != 0.0f) {
        l_j /= (float)p_mainfold.m_contacts.size();
    }

    glm::vec3 l_impulse = l_relativeNorm * l_j;
    p_objectA.m_rigidBody.m_velocity -= l_impulse * l_invMassA;
    p_objectA.m_rigidBody.m_angularVelocity -= glm::cross(l_rA, l_impulse) * l_interiaTensorA;
    if(p_objectB.m_isRigidBody)
    {
        p_objectB.m_rigidBody.m_velocity +=  l_impulse * l_invMassB;
        p_objectB.m_rigidBody.m_angularVelocity += glm::cross(l_rB, l_impulse) * l_interiaTensorB;
    }

    glm::vec3 l_t = l_relativeVel - (l_relativeNorm *
        glm::dot(l_relativeVel, l_relativeNorm));
    if (cmp(glm::length(l_t) * glm::length(l_t), 0.0f)) {
        return;
    }

    l_t = glm::normalize(l_t);

    l_numerator = -glm::dot(l_relativeVel, l_t);

    l_d = l_invMassSum;
    l_dvecA = glm::cross(glm::cross(l_rA, l_t) * l_interiaTensorA, l_rA);
    l_dvecB = glm::cross(glm::cross(l_rB, l_t) * l_interiaTensorB, l_rB);
    l_denominator = l_d + glm::dot(l_t, l_dvecA + l_dvecB);

    if (l_denominator == 0.0f)
    {
        return;
    }

    float l_jt = l_numerator / l_denominator;
    if (p_mainfold.m_contacts.size() > 0.0f and l_jt != 0.0f) 
    {
        l_jt /= (float)p_mainfold.m_contacts.size();
    }

    if (cmp(l_jt, 0.0f)) 
    {
        return;
    }

    float l_friction = sqrtf(p_objectA.m_rigidBody.m_materialProperties.m_frction * 
                             p_objectB.m_rigidBody.m_materialProperties.m_frction);
    if (l_jt > l_j * l_friction) {
        l_jt = l_j * l_friction;
    }
    else if (l_jt < -l_j * l_friction) {
        l_jt = -l_j * l_friction;
    }

    glm::vec3 l_tangentImpuse = l_t * l_jt;
    p_objectA.m_rigidBody.m_velocity -= l_tangentImpuse * l_invMassA;
    p_objectA.m_rigidBody.m_angularVelocity -= glm::cross(l_rA, l_tangentImpuse) * l_interiaTensorA;
    if(p_objectB.m_isRigidBody)
    {
        p_objectB.m_rigidBody.m_velocity += l_tangentImpuse * l_invMassB;
        p_objectB.m_rigidBody.m_angularVelocity += glm::cross(l_rB, l_tangentImpuse) * l_interiaTensorB;
    }
}

void Geometry3dUtils::applyRotationeImpulse(Meshes::Object & p_object,
                                            const glm::vec3& p_point, 
                                            const glm::vec3& p_impulse)
{
    glm::vec3 l_centerMass = p_object.m_transform.m_position;

    glm::vec3 l_torque = glm::cross(p_point - l_centerMass, p_impulse);

    p_object.m_rigidBody.m_torque = l_torque;

    glm::vec3 l_angularAcceleration = 
        l_torque * glm::mat3(p_object.m_rigidBody.m_invBodyTensorOfInteria);

    p_object.m_rigidBody.m_angularVelocity += l_angularAcceleration;
}

}//Utils
}//McEngine
