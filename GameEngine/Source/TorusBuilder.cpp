#include "TorusBuilder.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{
    ObjectBuilder & TorusBuilder::addMesh()
    {
        try
        {
            m_object.m_meshes.push_back(PrefabManager::getInstance().getMesh("Torus"));
        }
        catch (std::exception& ex)
        {
            std::string err = ex.what();
            LOG(err, LogType::ERR);
        }
        return *this;
    }
    ObjectBuilder & TorusBuilder::addShaderProgram(std::string p_shaderLabel)
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
    Object TorusBuilder::getObject() const
    {
        return m_object;
    }
}//Meshes
}//McEngine
