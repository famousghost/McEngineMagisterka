#include "ColiderBuilder.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{

ObjectBuilder & ColiderBuilder::addMesh()
{
    try
    {
        m_object.m_meshes.push_back(PrefabManager::getInstance().getMesh("CubeColider"));

    }
    catch (std::exception& ex)
    {
        std::string err = ex.what();
        LOG(err, LogType::ERR);
    }
    return *this;
}
ObjectBuilder & ColiderBuilder::addShaderProgram(std::string p_shaderLabel)
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
Object ColiderBuilder::getObject()
{
    return m_object;
}

}//Meshes
}//McEngine