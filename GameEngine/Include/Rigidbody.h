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
    glm::vec3 m_velocity;
    glm::vec3 m_force;
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

