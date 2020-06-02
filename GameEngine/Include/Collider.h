#pragma once
#include "IColiderObserver.h"
#include "Utilities.h"

namespace McEngine
{
namespace Meshes
{
struct Collider
{
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::shared_ptr<Shaders::Shader> m_shaderProgram;
    std::vector<glm::vec4> m_verticies;
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec4 m_minVertex;
    glm::vec4 m_maxVertex;
    Section m_xSection;
    Section m_ySection;
    Section m_zSection;
    Normals m_normals;
    glm::vec3 m_coliderColor;

    Collider()
    {
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_coliderColor = glm::vec3(0.0f, 1.0f, 0.0f);

        setDefaultValues();
    }

    void setDefaultValues()
    {
        m_minVertex = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
        m_maxVertex = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);

        m_xSection.min = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
        m_xSection.max = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);

        m_ySection.min = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
        m_ySection.max = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);

        m_zSection.min = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
        m_zSection.max = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);

        m_normals.x = glm::vec3(1.0f, 0.0f, 0.0f);
        m_normals.y = glm::vec3(0.0f, 1.0f, 0.0f);
        m_normals.z = glm::vec3(0.0f, 0.0f, 1.0f);

        std::vector<glm::vec4> l_verticies =
        {
            glm::vec4{-1.0f, 1.0f, 1.0f, 1.0f},
            glm::vec4{-1.0f, -1.0f, 1.0f, 1.0f},
            glm::vec4{1.0f, -1.0f, 1.0f, 1.0f},
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
            glm::vec4{-1.0f, 1.0f, -1.0f, 1.0f},
            glm::vec4{-1.0f, -1.0f, -1.0f, 1.0f},
            glm::vec4{1.0f, -1.0f, -1.0f, 1.0f},
            glm::vec4{1.0f, 1.0f, -1.0f, 1.0f}
        };
        m_verticies = l_verticies;
    }
};
}//Meshes
}//McEngine

