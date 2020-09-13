#pragma once
#include "Object.h"
#include "ColMainfold.h"

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
    glm::vec3 calculateRealCollsionDirection(const glm::vec3 & p_collsionDirection);

    void collisionChecker(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

private:
    bool checkCollsionForObject(const Meshes::Collider & p_colliderA, 
                                const Meshes::Collider & p_ColliderB, 
                                const glm::vec3 & p_objectCenterA, 
                                const glm::vec3 & p_objectCenterB, 
                                const Meshes::Object * p_objectA, 
                                const Meshes::Object * p_objectB);
    bool m_shouldCheckCollsion;
    Meshes::ColMainfold m_colMainfold;
    glm::vec3 m_collsionDirection;
};
}//Physics
}//McEngine

