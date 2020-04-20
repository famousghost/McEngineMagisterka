#pragma once
#include <memory>
#include "Object.h"
#include "ObjectBuilder.h"
#include "ModelBuilder.h"
namespace McEngine
{
namespace Meshes
{
class CustomObjectBuilder : public ObjectBuilder
{
public:
    CustomObjectBuilder(std::string p_filePath);

    ObjectBuilder& addMesh() override;
    ObjectBuilder& addShaderProgram(std::string p_shaderLabel) override;

    Object getObject() const override;
private:
    Object m_object;
    std::string m_filePath;
    
};

}//Meshes
}//McEngine

