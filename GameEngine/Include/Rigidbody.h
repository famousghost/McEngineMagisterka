#pragma once
#include "Mass.h"
#include "MaterialProperties.h"
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Meshes
{
struct Rigidbody
{
    Mass m_massProperties;

    glm::dmat3 m_ibody;
    glm::dmat3 m_inverseIbody;

    glm::dvec3 m_x; /*x(t)*/
    glm::dmat3 m_R; /*R(t)*/
    glm::dvec3 m_P; /*P(t)*/
    glm::dvec3 m_L; /*L(t)*/

    glm::dmat3 m_iInv;
    glm::vec3 m_velocity;
    glm::dvec3 m_angularVelocity;

    glm::dvec3 m_force;
    glm::dvec3 m_torque;

    std::vector<double> m_y;
    std::vector<double> m_yFinal;

    double m_gravity;
    MaterialProperties m_materialProperties;
    float m_width;
    float m_height;
    float m_length;


    Rigidbody()
    {
        m_width = 1.0f;
        m_height = 1.0f;
        m_length = 1.0f;
        m_gravity = -9.87;
    }
};

}//Meshes
}//McEngine

