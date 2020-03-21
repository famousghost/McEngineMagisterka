#include "CubeBuilder.h"
#include "Logger.h"
#include "VAOManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{

ObjectBuilder& CubeBuilder::addVertexArrayObject()
{
    try
    {
        m_object.m_vertexArray = VAOManager::getInstance().getVertexArray("CUBE");
    }
    catch (std::exception& ex)
    {
        std::string err = ex.what();
        LOG(err, LogType::ERR);
    }
    return *this;
}

ObjectBuilder& CubeBuilder::addShaderProgram(std::string p_shaderLabel)
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

ObjectBuilder& CubeBuilder::addTextures()
{
    return *this;
}

Object CubeBuilder::getObject() const
{
    return m_object;
}

}//Meshes
}//GameEngine