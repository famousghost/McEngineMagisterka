#pragma once
#include "Object.h"

namespace McEngine
{
namespace Physics
{
class PhysicsManager
{
public:
    void start();

    void shutdown();

    static PhysicsManager& getInstance();

    bool checkCollision(const Meshes::Colider & p_coliderA, const Meshes::Colider & p_coliderB);
};
}//Physics
}//McEngine

