#pragma once
#include "MeshCollisionHandler.h"
namespace McEngine
{
namespace Physics
{

class CubeOBBCollsionHandler : public MeshCollisionHandler
{
public:

    bool checkCollision(const Meshes::Collider & p_coliderA, const Meshes::Collider & p_coliderB) override;
private:
    std::vector<double> getProjectedPointsToAxis(const std::vector<glm::vec4>& p_listOfVec, const glm::vec3 & p_normals);
    bool checkCollisionForAxis(const std::vector<double>& p_projectedColliderA, const std::vector<double>& p_projectedColliderB);
    bool checkCollisionForNormalAxis(const std::vector<double>& p_projectedColliderAX, const std::vector<double>& p_projectedColliderAY, const std::vector<double>& p_projectedColliderAZ, const std::vector<double>& p_projectedColliderBX, const std::vector<double>& p_projectedColliderBY, const std::vector<double>& p_projectedColliderBZ);

};

}//Physics
}//McEngine

