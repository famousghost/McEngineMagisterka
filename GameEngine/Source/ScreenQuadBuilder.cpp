#include "ScreenQuadBuilder.h"
#include "Logger.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{

ObjectBuilder& ScreenQuadBuilder::addMesh()
{
    m_object.m_meshes.push_back(PrefabManager::getInstance().getScreenQuadMesh());
    return *this;
}

ObjectBuilder& ScreenQuadBuilder::addShaderProgram(std::string p_shaderLabel)
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

Object ScreenQuadBuilder::getObject()
{
    return m_object;
}

}//Meshes
}//McEngine