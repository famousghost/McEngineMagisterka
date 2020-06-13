#pragma once
#include "Object.h"
#include "CollisionHandler.h"

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

    void collisionChecker(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    void setShouldCheckCollision(bool p_shouldCheckCollision);
    bool getShouldCheckCollsion() const;
private:
    CollisionHandler m_collsionHandler;
};
}//Physics
}//McEngine

