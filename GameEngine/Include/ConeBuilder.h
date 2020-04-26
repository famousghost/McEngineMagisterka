#pragma once
#include "ObjectBuilder.h"
namespace McEngine
{
namespace Meshes
{

class ConeBuilder : public ObjectBuilder
{
public:
    ConeBuilder()=default;
    ObjectBuilder& addMesh() override;
    ObjectBuilder& addShaderProgram(std::string p_shaderLabel) override;

    Object getObject() const override;
private:
    Object m_object;
};

}//Meshes
}//McEngine