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

    void checkCollisions(Meshes::Object & p_object, 
                         std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    void setShouldCheckCollision(bool p_shouldCheckCollision);
    bool getShouldCheckCollsion() const;
private:
    bool checkCollision(const Meshes::Colider & p_coliderA, const Meshes::Colider & p_coliderB);
    void collisionChecker(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    bool m_shouldCheckCollsion;
};
}//Physics
}//McEngine

