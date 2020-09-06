#pragma once
#include "IRigidbodyHandler.h"
#include "Rigidbody.h"
#include <memory>

namespace McEngine
{
namespace Physics
{

class RigidbodyHandlerOBB : public IRigidbodyHandler
{
public:

    RigidbodyHandlerOBB(Meshes::Rigidbody* p_rigidbody);

    ~RigidbodyHandlerOBB();
    
    void update() override;

    void solveConstraints(const std::vector<std::pair<Meshes::Object, std::string>>& p_constraintObjects) override;

    void applyForces() override;

private:
    void applyGravityForce();

    Meshes::Rigidbody* m_rigidbody;
    glm::vec3 m_forces;
};

}//Physics
}//McEngine

