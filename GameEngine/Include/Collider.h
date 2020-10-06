#pragma once
#include "IColiderObserver.h"
#include "Utilities.h"
#include "Transform.h"

namespace McEngine
{
namespace Meshes
{
enum class ColliderType
{
    NO_COLLIDER,
    CUBE_AABB,
    CUBE_OBB,
    SPHERE,
};

struct Collider
{
    ColliderType m_colliderType;
    std::string m_colliderName;
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::shared_ptr<Shaders::Shader> m_shaderProgram;
    std::vector<glm::vec4> m_verticies;
    glm::mat4 m_modelMatrix;
    float m_width;
    float m_height;
    float m_length;
    Transform m_transform;
    glm::vec4 m_minVertex;
    glm::vec4 m_maxVertex;
    Section m_xSection;
    Section m_ySection;
    Section m_zSection;
    Normals m_normals;
    float m_radius;
    glm::vec3 m_coliderColor;

    Collider()
    {
        m_colliderType = ColliderType::SPHERE;
        m_transform.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_transform.m_rotatione = glm::vec3(0.0f, 0.0f, 0.0f);
        m_transform.m_scale = glm::vec3(0.0f, 0.0f, 0.0f);
        m_coliderColor = glm::vec3(0.0f, 1.0f, 0.0f);
        m_radius = 1.0f;
        setDefaultValues();
    }

    void setDefaultValues()
    {
        m_modelMatrix = glm::mat4();
        m_minVertex = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
        m_maxVertex = glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);

        m_xSection.min = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
        m_xSection.max = glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);

        m_ySection.min = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
        m_ySection.max = glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);

        m_width = 1.0f;
        m_height = 1.0f;
        m_length = 1.0f;

        m_zSection.min = glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
        m_zSection.max = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);

        m_normals.x = glm::vec3(1.0f, 0.0f, 0.0f);
        m_normals.y = glm::vec3(0.0f, 1.0f, 0.0f);
        m_normals.z = glm::vec3(0.0f, 0.0f, -1.0f);

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

