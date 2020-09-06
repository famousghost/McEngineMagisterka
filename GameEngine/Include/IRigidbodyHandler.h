#pragma once
#include "Object.h"

namespace McEngine
{
namespace Physics
{ 

class IRigidbodyHandler
{
public:
    IRigidbodyHandler(){}

    virtual void update() = 0;

    virtual void solveConstraints(const std::vector<std::pair<Meshes::Object, std::string>>&) = 0;

    virtual void applyForces() = 0;
};

}//Physics
}//McEngine

