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
    std::vector<double> getProjectedPointsToAxisX(const std::vector<glm::vec4>& p_listOfVec, const Meshes::Normals & p_normals);
    std::vector<double> getProjectedPointsToAxisY(const std::vector<glm::vec4>& p_listOfVec, const Meshes::Normals & p_normals);
    std::vector<double> getProjectedPointsToAxisZ(const std::vector<glm::vec4>& p_listOfVec, const Meshes::Normals & p_normals);
    bool checkCollisionForAxis(const std::vector<double>& p_projectedColliderAX, const std::vector<double>& p_projectedColliderAY, const std::vector<double>& p_projectedColliderAZ, const std::vector<double>& p_projectedColliderBX, const std::vector<double>& p_projectedColliderBY, const std::vector<double>& p_projectedColliderBZ);
    bool checkCollisionOBB(const Meshes::Colider & p_coliderA, const Meshes::Colider & p_coliderB);
    void print(const glm::vec4 & p_vec);
    void collisionCheckerAABB(Meshes::Object & p_object, std::vector<std::pair<Meshes::Object, std::string>>& p_objects);

    bool m_shouldCheckCollsion;
};
}//Physics
}//McEngine

