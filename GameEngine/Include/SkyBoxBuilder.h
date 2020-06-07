#pragma once
#include <memory>
#include "Object.h"
#include "ObjectBuilder.h"
#include "ModelBuilder.h"
namespace McEngine
{
namespace Meshes
{
class SkyBoxBuilder : public ObjectBuilder
{
public:
    SkyBoxBuilder() = default;
    ObjectBuilder& addMesh() override;
    ObjectBuilder& addShaderProgram(std::string p_shaderLabel) override;

    Object getObject() override;
private:
    Object m_object;
};

}//Meshes
}//McEngine

