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
    Mass m_mass;
    glm::vec3 m_velocity;
    glm::vec3 m_force;
    double m_gravity;
    MaterialProperties m_materialProperties;


    Rigidbody()
    {
        m_gravity = -9.87;
    }
};

}//Meshes
}//McEngine

