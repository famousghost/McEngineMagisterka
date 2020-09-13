#pragma once
#include "Object.h"
#include "ColMainfold.h"

namespace McEngine
{
namespace Physics
{

class MeshCollisionHandler
{
public:
    virtual bool checkCollision(const Meshes::Collider & p_coliderA, 
                                const Meshes::Collider & p_coliderB) = 0;
    
    virtual Meshes::ColMainfold getColMainfold() const = 0;
};

}//Physics
}//McEngine

