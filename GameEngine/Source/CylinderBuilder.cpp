#include "CylinderBuilder.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{
ObjectBuilder & CylinderBuilder::addMesh()
{
    try
    {
        m_object.m_meshes.push_back(PrefabManager::getInstance().getMesh("Cylinder"));
    }
    catch (std::exception& ex)
    {
        std::string err = ex.what();
        LOG(err, LogType::ERR);
    }
    return *this;
}
ObjectBuilder & CylinderBuilder::addShaderProgram(std::string p_shaderLabel)
{
    try
    {
        m_object.m_shaderProgram = Shaders::ShaderManager::getInstance().getShader(p_shaderLabel);
    }
    catch (std::exception& ex)
    {
        std::string err = ex.what();
        LOG(err, LogType::ERR);
    }
    return *this;
}
Object CylinderBuilder::getObject() const
{
    return m_object;
}

}//Meshes
}//McEngine