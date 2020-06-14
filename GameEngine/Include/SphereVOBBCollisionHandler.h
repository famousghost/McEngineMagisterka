#pragma once
#include "MeshCollisionHandler.h"

namespace McEngine
{
namespace Physics
{

class SphereVOBBCollisionHandler : public MeshCollisionHandler
{
public:
    SphereVOBBCollisionHandler(float p_distance);

    bool checkCollision(const Meshes::Collider & p_coliderA,
                        const Meshes::Collider & p_coliderB) override;

private:
    float m_distance;
};

}//Physics
}//McEngine