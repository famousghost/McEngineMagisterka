#pragma once
#include "ObjectBuilder.h"

namespace McEngine
{
namespace Meshes
{

class LoadedObjectBuilder : public ObjectBuilder
{
public:
    LoadedObjectBuilder(std::string p_objectName);
    ObjectBuilder& addMesh() override;
    ObjectBuilder& addShaderProgram(std::string p_shaderLabel) override;

    Object getObject() const override;
private:
    Object m_object;
    std::string m_objectName;
};
}//Meshes
}//McEngine

