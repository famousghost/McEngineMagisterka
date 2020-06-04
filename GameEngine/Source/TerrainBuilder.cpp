#include "TerrainBuilder.h"
#include "Logger.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{

ObjectBuilder& TerrainBuilder::addMesh()
{
    m_object.m_meshes.push_back(PrefabManager::getInstance().getTerrainMesh());
    m_object.m_transform.m_scale = glm::vec3(50.0f, 50.0f, 50.0f);
    return *this;
}

ObjectBuilder& TerrainBuilder::addShaderProgram(std::string p_shaderLabel)
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

Object TerrainBuilder::getObject()
{
    return m_object;
}

}//Meshes
}//McEngine