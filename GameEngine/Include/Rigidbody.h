#pragma once
#include "Mass.h"
#include "Ray.h"
#include "MaterialProperties.h"
#include <matrix_transform.hpp>
#include <type_ptr.hpp>
#include "../glm-0.9.6.3/glm/glm/gtx/quaternion.hpp"
#include "../glm-0.9.6.3/glm/glm/common.hpp"

namespace McEngine
{
namespace Meshes
{
struct Rigidbody
{
    Mass m_massProperties;

    glm::mat3 m_ibody;
    glm::mat3 m_inverseIbody;

    glm::vec3* m_position; /*x(t)*/
    glm::vec3 m_prevPosition; /*prev x(t)*/
    glm::vec3 m_P; /*P(t)*/
    glm::vec3 m_L; /*L(t)*/
    glm::quat m_quat; /*quaternion rotatione*/

    glm::vec3 m_dx /*dx/dt -> changing position in time (velocity)*/;
    glm::vec3 m_dP /*dP/dt -> changnig forces in time*/;
    glm::vec3 m_dL /*dL/dt -> torque*/;
    glm::quat m_quatDt; /*derived from quaternion rotatione*/

    glm::mat3 m_iWordlInv;
    glm::vec3 m_velocity;
    glm::vec3 m_angularVelocity;
    glm::vec3 m_oldVelocity;

    glm::vec3 m_force;
    glm::vec3 m_torque;

    float m_gravity;
    MaterialProperties m_materialProperties;
    float m_width;
    float m_height;
    float m_length;

    bool m_gravityForce;
    bool m_isOnGrounded;


    Rigidbody()
    {
        m_width = 1.0f;
        m_height = 1.0f;
        m_length = 1.0f;
        m_gravity = -9.87;
        m_velocity = glm::vec3();
        m_gravityForce = false;
        m_isOnGrounded = false;
    }

};

}//Meshes
}//McEngine

