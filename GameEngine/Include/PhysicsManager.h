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
    void updateBodyState(Meshes::Rigidbody & p_rigidBody);
    void dbgVector(const glm::vec3 & p_vec, const std::string & p_msg);
    void ode(Meshes::Object & p_object);
    void computeForceAndTorque(Meshes::Object & p_object);
    glm::mat3 starOperatorMatrix(const glm::vec3 & p_vec);
    void debugMatrix(const glm::dmat3& p_mat);
    void calculateDerivates(Meshes::Rigidbody & p_rigidBody);
    bool getShouldCheckCollsion() const;
    void updatePhysics(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    int getStateSize() const;
private:
    float calculateObjectVolume(Meshes::Object & p_object);
    void collisionChecker(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    CollisionHandler m_collsionHandler;

    const int m_stateSize = 18;
};
}//Physics
}//McEngine

