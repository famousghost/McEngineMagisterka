#pragma once
#include "MeshCollisionHandler.h"

namespace McEngine
{
namespace Physics
{

class SphereVOBBCollisionHandler : public MeshCollisionHandler
{
public:
    SphereVOBBCollisionHandler(const glm::vec3 & p_objectCenterA,
                               const glm::vec3 & p_objectCenterB);

    bool checkCollision(const Meshes::Collider & p_coliderA,
                        const Meshes::Collider & p_coliderB) override;

private:
    glm::vec3 m_objectCenterA;
    glm::vec3 m_objectCenterB;
};

}//Physics
}//McEngine