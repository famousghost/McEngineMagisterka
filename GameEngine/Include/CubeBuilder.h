#pragma once
#include <memory>
#include "Object.h"
#include "ObjectBuilder.h"
#include "ModelBuilder.h"

namespace McEngine
{
namespace Meshes
{
class CubeBuilder : public ObjectBuilder
{
public:
    CubeBuilder() = default;
    ObjectBuilder& addVertexArrayObject() override;
    ObjectBuilder& addShaderProgram(std::string p_shaderLabel) override;
    ObjectBuilder& addTextures() override;
    Object getObject() const override;
private:
    Object m_object;
};

}//Meshes
}//GameEngine