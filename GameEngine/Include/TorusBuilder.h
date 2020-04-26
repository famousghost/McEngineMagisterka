#pragma once
#include "ObjectBuilder.h"
namespace McEngine
{
namespace Meshes
{
class TorusBuilder : public ObjectBuilder
{
public:
    TorusBuilder()=default;
    ObjectBuilder& addMesh() override;
    ObjectBuilder& addShaderProgram(std::string p_shaderLabel) override;

    Object getObject() const override;
private:
    Object m_object;
};

}//Meshes
}//McEngine