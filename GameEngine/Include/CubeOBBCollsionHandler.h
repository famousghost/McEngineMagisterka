#pragma once
#include "MeshCollisionHandler.h"
#include "LineSegment.h"
#include "Plane.h"

namespace McEngine
{
namespace Physics
{

class CubeOBBCollsionHandler : public MeshCollisionHandler
{
public:
    CubeOBBCollsionHandler(glm::vec3 & p_minimumTranslationVector, 
                           const glm::vec3 & p_objectACenter, 
                           const glm::vec3 & p_objectBCenter,
                           const Meshes::Object* p_objectA,
                           const Meshes::Object* p_objectB);

    std::vector<Meshes::LineSegment> getEdges(const Meshes::Collider & p_collider);

    std::vector<Meshes::Plane> getFaces(const Meshes::Collider & p_collider, 
                                        const Meshes::Object & p_object);

    std::vector<glm::vec3> clipEdgesToCube(const std::vector<Meshes::LineSegment>& p_edges, 
                                           const Meshes::Collider & p_collider,
                                           const Meshes::Object& p_object);

    float findPenetrationDepth(const Meshes::Object & p_colliderA,
                               const Meshes::Object & p_colliderB,
                               const glm::vec3 & p_axis, 
                               bool & p_shouldFlip);

    bool checkCollision(const Meshes::Collider & p_coliderA, 
                        const Meshes::Collider & p_coliderB) override;

    Meshes::ColMainfold findCollsionFeatures(const Meshes::Object& p_objectA,
                                             const Meshes::Object& p_objectB);

    Meshes::ColMainfold getColMainfold() const override;
private:
    glm::vec3& m_minimumTranslationVector;
    float m_minOverlap;

    glm::vec3 m_objectCenterA;
    glm::vec3 m_objectCenterB;
    Meshes::ColMainfold m_colMainfold;
    const Meshes::Object* m_objectA;
    const Meshes::Object* m_objectB;
};

}//Physics
}//McEngine

