#pragma once
#include "ObjectBuilder.h"

namespace McEngine
{
namespace Meshes
{

class LoadedObjectBuilder : public ObjectBuilder
{
public:
    LoadedObjectBuilder(std::string p_meshName, std::string p_objectName);
    LoadedObjectBuilder(std::string p_meshName, std::string p_objectName, ColliderType p_defaultColliderType);
    ObjectBuilder& addMesh() override;
    ObjectBuilder& addShaderProgram(std::string p_shaderLabel) override;

    Object getObject() override;
private:
    Object m_object;
    std::string m_meshName;
    std::string m_objectName;
    ColliderType m_defaultColliderType;
};
}//Meshes
}//McEngine

