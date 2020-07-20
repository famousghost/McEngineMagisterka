#pragma once
#include "Logger.h"

namespace McEngine
{
namespace Meshes
{
enum class MaterialType
{
    ROCK,      //Density : 0.6  Restitution : 0.1
    WOOD,      //Density : 0.3  Restitution : 0.2
    METAL,      //Density : 1.2  Restitution : 0.05
    BOUNCY_BALL, //Density : 0.3  Restitution : 0.8
    SUPER_BALL,  //Density : 0.3  Restitution : 0.95
    PILLOW,    //Density : 0.1  Restitution : 0.2
    STATIC,    //Density : 0.0  Restitution : 0.4
};

struct MaterialProperties
{
    MaterialType m_materialType;
    float m_density;
    float m_restitution;

    MaterialProperties()
    {
        m_density = 0.6f;
        m_restitution = 0.1f;
    }

    void setMaterialType(const std::string& p_materialTypeName)
    {
        if (p_materialTypeName == "ROCK")
        {
            m_materialType = MaterialType::ROCK;
        }
        else if (p_materialTypeName == "WOOD")
        {
            m_materialType = MaterialType::WOOD;
        }
        else if (p_materialTypeName == "METAL")
        {
            m_materialType = MaterialType::METAL;
        }
        else if (p_materialTypeName == "BOUNCY_BALL")
        {
            m_materialType = MaterialType::BOUNCY_BALL;
        }
        else if (p_materialTypeName == "SUPER_BALL")
        {
            m_materialType = MaterialType::SUPER_BALL;
        }
        else if (p_materialTypeName == "PILLOW")
        {
            m_materialType = MaterialType::PILLOW;
        }
        else
        {
            m_materialType = MaterialType::STATIC;
        }

        setMaterialProperties(m_materialType);
    }

    void setMaterialProperties(MaterialType p_materialType)
    {
        switch (p_materialType)
        {
            case McEngine::Meshes::MaterialType::ROCK:
                m_density = 0.6f;
                m_restitution = 0.1f;
                break;
            case McEngine::Meshes::MaterialType::WOOD:
                m_density = 0.3f;
                m_restitution = 0.2f;
                break;
            case McEngine::Meshes::MaterialType::METAL:
                m_density = 1.2f;
                m_restitution = 0.05f;
                break;
            case McEngine::Meshes::MaterialType::BOUNCY_BALL:
                m_density = 0.3f;
                m_restitution = 0.8f;
                break;
            case McEngine::Meshes::MaterialType::SUPER_BALL:
                m_density = 0.3f;
                m_restitution = 0.95f;
                break;
            case McEngine::Meshes::MaterialType::PILLOW:
                m_density = 0.1f;
                m_restitution = 0.2f;
                break;
            case McEngine::Meshes::MaterialType::STATIC:
                m_density = 0.0f;
                m_restitution = 0.4f;
                break;
            default:
                LOG("Cannot find material type", LogType::WARN);
                break;
        }
    }
};
}//Meshes
}//McEngine

