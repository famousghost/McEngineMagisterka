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

    Meshes::ColMainfold getColMainfold() const override;


private:
    float calculateDistanceBetweenClosestPointAndSphereCeneter(const Meshes::Collider & p_cubeCollider, 
                                                               const Meshes::Collider & p_sphereCoolider, 
                                                               const glm::vec3 & p_sphereCenter, 
                                                               const glm::vec3 & p_cubePosition);


    glm::vec3 m_objectCenterA;
    glm::vec3 m_objectCenterB;
    glm::vec3 m_collsionDirection;
    Meshes::ColMainfold m_colMainfold;

};

}//Physics
}//McEngine