#pragma once
#include "Object.h"
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Meshes
{

class ObjectBuilder
{
    public:
        virtual ~ObjectBuilder(){}
        virtual ObjectBuilder& addVertexArrayObject() = 0;
        virtual ObjectBuilder& addShaderProgram(std::string p_shaderLabel) = 0;
        virtual ObjectBuilder& addTextures() = 0;

        virtual Object getObject() const = 0;
};

}//Meshes
}//GameEngine