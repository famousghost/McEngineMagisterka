#include "TerrainBuilder.h"
#include "Logger.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{

TerrainBuilder::TerrainBuilder(ColliderType p_defaultColliderType)
{
    m_object.m_colider.emplace_back();
    m_object.m_colider.back().m_colliderType = p_defaultColliderType;
}

ObjectBuilder& TerrainBuilder::addMesh()
{
    auto& l_prefabManager = PrefabManager::getInstance();
    m_object.m_meshes.push_back(PrefabManager::getInstance().getTerrainMesh());
    m_object.m_transform.m_scale = glm::vec3(50.0f, 50.0f, 50.0f);
    m_object.m_colider.back().m_colliderName = "Collider" + std::to_string(m_object.m_colliderNumber++);
    if (m_object.m_colider.back().m_colliderType == ColliderType::SPHERE)
    {
        m_object.m_colider.back().m_meshes.push_back(l_prefabManager.getMesh("Sphere"));
    }
    else
    {
        m_object.m_colider.back().m_meshes.push_back(l_prefabManager.getColiderMesh());
    }
    return *this;
}

ObjectBuilder& TerrainBuilder::addShaderProgram(std::string p_shaderLabel)
{
    try
    {
        m_object.m_currentActiveShader = p_shaderLabel;
        m_object.m_shaderProgram = Shaders::ShaderManager::getInstance().getShader(p_shaderLabel);
        m_object.m_colider.back().m_shaderProgram = Shaders::ShaderManager::getInstance().getShader("coliderShader");
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