#include "ConeBuilder.h"
#include "PrefabManager.h"
#include "ShaderManager.h"

namespace McEngine
{
namespace Meshes
{
    ObjectBuilder & ConeBuilder::addMesh()
    {
        try
        {
            m_object.m_meshes.push_back(PrefabManager::getInstance().getMesh("Cone"));
        }
        catch (std::exception& ex)
        {
            std::string err = ex.what();
            LOG(err, LogType::ERR);
        }
        return *this;
    }
    ObjectBuilder & ConeBuilder::addShaderProgram(std::string p_shaderLabel)
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

    Object ConeBuilder::getObject() const
    {
        return m_object;
    }
}//Meshes
}//McEngine