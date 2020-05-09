#pragma once
#include "Mesh.h"
#include "Shader.h"
#include <memory>
#include <string>
#include <map>

namespace McEngine
{
namespace Meshes
{
struct Material
{
    Material()
    {
        m_ambientLight = glm::vec3(0.1f, 0.1f, 0.1f);
        m_diffuseLight = glm::vec3(0.1f, 0.1f, 0.1f);
        m_specularLight = glm::vec3(0.1f, 0.1f, 0.1f);
        m_objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    glm::vec3 m_ambientLight;
    glm::vec3 m_diffuseLight;
    glm::vec3 m_specularLight;
    glm::vec3 m_objectColor;
};

struct Transform
{
    Transform()
    {
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotatione = glm::vec3(0.0f, 0.0f, 0.0f);
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }
    glm::vec3 m_position;
    glm::vec3 m_rotatione;
    glm::vec3 m_scale;
};

struct Colider
{
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::shared_ptr<Shaders::Shader> m_shaderProgram;
    glm::mat4 m_modelMatrix;
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::vec4 m_firstVertex;
    glm::vec4 m_rawFirstVertex;
    glm::vec3 m_coliderColor;
    float m_width;
    float m_heigth;
    float m_length;

    Colider()
    {
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_coliderColor = glm::vec3(0.0f, 1.0f ,0.0f);
        m_width = 2.0f;
        m_heigth = 2.0f;
        m_length = 2.0f;
        m_modelMatrix = glm::mat4();
    }
};

struct Object
{
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::shared_ptr<Shaders::Shader> m_shaderProgram;
    Colider m_colider;
    std::string m_currentAvaiableTexture;
    Material m_material;
    glm::mat4 m_modelMatrix;
    Transform m_transform;

    Object()
    {
        m_modelMatrix = glm::mat4();
        m_currentAvaiableTexture = "texture";
    }
};

}//Meshes
}//McEngine

