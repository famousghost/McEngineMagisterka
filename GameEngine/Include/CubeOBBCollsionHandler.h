#pragma once
#include "MeshCollisionHandler.h"
namespace McEngine
{
namespace Physics
{

class CubeOBBCollsionHandler : public MeshCollisionHandler
{
public:
    CubeOBBCollsionHandler(glm::vec3 & p_minimumTranslationVector, const glm::vec3 & p_objectACenter, const glm::vec3 & p_objectBCenter);

    bool checkCollision(const Meshes::Collider & p_coliderA, const Meshes::Collider & p_coliderB) override;
private:
    std::vector<double> getProjectedPointsToAxis(const std::vector<glm::vec4>& p_listOfVec, const glm::vec3 & p_normals);
    bool checkCollisionForNormalsAxis(const Meshes::Collider & p_coliderA, const Meshes::Collider & p_coliderB, const std::vector<glm::vec3>& p_colliderAEdges, const std::vector<glm::vec3>& p_colliderBEdges);
    double checkCollisionForNormalAxis(const std::vector<double>& p_projectedColliderA, const std::vector<double>& p_projectedColliderB);
    double checkCollisionForAxis(const std::vector<double>& p_projectedColliderA, const std::vector<double>& p_projectedColliderB);
    glm::vec3& m_minimumTranslationVector;
    float m_minGap;

    glm::vec3 m_objectCenterA;
    glm::vec3 m_objectCenterB;
};

}//Physics
}//McEngine

