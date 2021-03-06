#pragma once
#include "MeshCollisionHandler.h"
namespace McEngine
{
namespace Physics
{

class CubeAABBCollisionHandler : public MeshCollisionHandler
{
public:   
    bool checkCollision(const Meshes::Collider & p_coliderA, const Meshes::Collider & p_coliderB) override;

    Meshes::ColMainfold getColMainfold() const override;
    
    Meshes::ColMainfold m_mainfold;
};

}//Physics
}//McEngine

