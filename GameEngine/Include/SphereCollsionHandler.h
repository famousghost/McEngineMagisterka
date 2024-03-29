#pragma once
#include "MeshCollisionHandler.h"

namespace McEngine
{
namespace Physics
{

class SphereCollsionHandler : public MeshCollisionHandler
{
public:
    SphereCollsionHandler(const glm::vec3& p_objectCenterA, const glm::vec3& p_objectCenterB);

    bool checkCollision(const Meshes::Collider & p_coliderA, 
                        const Meshes::Collider & p_coliderB) override;

    Meshes::ColMainfold getColMainfold() const override;

private:
    const glm::vec3& m_objectCenterA;
    const glm::vec3& m_objectCenterB;
    Meshes::ColMainfold m_colMainfold;
};

}//Physics
}//McEngine

