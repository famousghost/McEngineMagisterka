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
                               const glm::vec3 & p_objectCenterB,
                               glm::vec3& p_collisionDirection);

    bool checkCollision(const Meshes::Collider & p_coliderA,
                        const Meshes::Collider & p_coliderB) override;


private:
    float calculateDistanceBetweenClosestPointAndSphereCeneter(const Meshes::Collider & p_cubeCollider, 
                                                               const Meshes::Collider & p_sphereCoolider, 
                                                               const glm::vec3 & p_sphereCenter);

    glm::vec3 m_objectCenterA;
    glm::vec3 m_objectCenterB;
    glm::vec3 m_collsionDirection;

};

}//Physics
}//McEngine