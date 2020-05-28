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

    void checkCollisionDetectionAABB(Meshes::Object & p_object, 
                                     std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    void checkCollisionDetectionOBB(Meshes::Object & p_object,
                                    std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    void collisionCheckerOBB(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    void setShouldCheckCollision(bool p_shouldCheckCollision);
    bool getShouldCheckCollsion() const;
private:
    bool checkCollisionAABB(const Meshes::Colider & p_coliderA, const Meshes::Colider & p_coliderB);
    bool checkCollisionOBB(const Meshes::Colider & p_coliderA, const Meshes::Colider & p_coliderB);
    void collisionCheckerAABB(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    bool m_shouldCheckCollsion;
};
}//Physics
}//McEngine

