#pragma once
#include "Object.h"

namespace McEngine
{
namespace Meshes
{

class ObjectBuilder
{
    public:
        virtual ~ObjectBuilder(){}
        virtual ObjectBuilder& addMesh() = 0;
        virtual ObjectBuilder& addShaderProgram(std::string p_shaderLabel) = 0;

        virtual Object getObject() = 0;
};

}//Meshes
}//GameEngine