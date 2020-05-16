#include "LoadedObjectBuilder.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{

LoadedObjectBuilder::LoadedObjectBuilder(std::string p_meshName, std::string p_objectName)
    :m_meshName(p_meshName),
     m_objectName(p_objectName)
{
}

ObjectBuilder & LoadedObjectBuilder::addMesh()
{
    try
    {
        auto& l_prefabManager = PrefabManager::getInstance();
        auto l_objectMeshes = l_prefabManager.getMeshes(m_meshName);
        m_object.m_meshes.insert(m_object.m_meshes.begin(), l_objectMeshes.begin(), l_objectMeshes.end());
        m_object.m_objectName = m_objectName;
        m_object.m_colider.m_meshes.push_back(l_prefabManager.getColiderMesh());
        m_object.m_colider.m_firstVertex = glm::vec4(m_object.m_colider.m_meshes.at(0)->m_verticies.at(0).m_position, 1.0f);
        m_object.m_colider.m_rawFirstVertex = m_object.m_colider.m_firstVertex;
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
        m_object.m_colider.m_shaderProgram = Shaders::ShaderManager::getInstance().getShader("coliderShader");
    }
    catch (std::exception& ex)
    {
        std::string err = ex.what();
        LOG(err, LogType::ERR);
    }
    return *this;
}

Object LoadedObjectBuilder::getObject()
{
    return m_object;
}

}//Meshes
}//McEngine
