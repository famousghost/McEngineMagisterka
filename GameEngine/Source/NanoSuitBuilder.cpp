#include "NanoSuitBuilder.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{
NanoSuitBuilder::NanoSuitBuilder(std::string p_filePath)
    :m_filePath(p_filePath)
    {}

ObjectBuilder & NanoSuitBuilder::addMesh()
{
    try
    {
        auto l_meshes = PrefabManager::getInstance().getMeshes(m_filePath);
        m_object.m_meshes.insert(m_object.m_meshes.begin(), l_meshes.begin(), l_meshes.end());
    }
    catch (std::exception& ex)
    {
        std::string err = ex.what();
        LOG(err, LogType::ERR);
    }
    return *this;
}

ObjectBuilder & NanoSuitBuilder::addShaderProgram(std::string p_shaderLabel)
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

Object NanoSuitBuilder::getObject() const
{
    return m_object;
}

}//Meshes
}//McEngine