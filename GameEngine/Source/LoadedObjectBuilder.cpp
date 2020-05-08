#include "LoadedObjectBuilder.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{

LoadedObjectBuilder::LoadedObjectBuilder(std::string p_objectName)
    :m_objectName(p_objectName)
{
}

ObjectBuilder & LoadedObjectBuilder::addMesh()
{
    try
    {
        auto l_meshes = PrefabManager::getInstance().getMeshes(m_objectName);
        m_object.m_meshes.insert(m_object.m_meshes.begin(), l_meshes.begin(), l_meshes.end());
    }
    catch (std::exception& ex)
    {
        std::string err = ex.what();
        LOG(err, LogType::ERR);
    }
    return *this;
}

ObjectBuilder & LoadedObjectBuilder::addShaderProgram(std::string p_shaderLabel)
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

Object LoadedObjectBuilder::getObject() const
{
    return m_object;
}

}//Meshes
}//McEngine
