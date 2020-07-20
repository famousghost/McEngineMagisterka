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

    void calculateObjectMass(Meshes::Object & p_object);

    void setShouldCheckCollision(bool p_shouldCheckCollision);
    bool getShouldCheckCollsion() const;
    void updatePhysics(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);
private:
    float calculateObjectVolume(Meshes::Object & p_object);
    void collisionChecker(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    CollisionHandler m_collsionHandler;
};
}//Physics
}//McEngine

