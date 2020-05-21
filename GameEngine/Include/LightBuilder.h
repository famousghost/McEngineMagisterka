#pragma once
#include "ObjectBuilder.h"

namespace McEngine
{
namespace Meshes
{

class LightBuilder : public ObjectBuilder
{
public:
    ObjectBuilder& addMesh() override;
    ObjectBuilder& addShaderProgram(std::string p_shaderLabel) override;

    Object getObject() override;
private:
    Object m_object;
};

}//Meshes
}//McEngine

