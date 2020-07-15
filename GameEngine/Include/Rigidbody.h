#pragma once

namespace McEngine
{
namespace Meshes
{
struct Rigidbody
{
    float m_mass;
    float m_velocity;
    float m_angularVelocity;



    Rigidbody()
    {
        m_mass = 1.0f;
        m_velocity = 0.0f;
        m_angularVelocity = 0.0f;
    }
};

}//Meshes
}//McEngine

