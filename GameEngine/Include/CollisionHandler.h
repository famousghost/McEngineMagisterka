#pragma once
#include "Object.h"

namespace McEngine
{
namespace Physics
{
class CollisionHandler
{
public:
    CollisionHandler();
    bool getShouldCheckCollsion() const;
    void setShouldCheckCollision(bool p_shouldCheckCollision);
    void collisionChecker(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

private:
    bool checkCollsionForObject(const Meshes::Collider & p_colliderA, const Meshes::Collider & p_ColliderB, float p_distanceBetweenObjects);
    bool m_shouldCheckCollsion;
};
}//Physics
}//McEngine

