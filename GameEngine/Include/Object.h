#pragma once
#include "Model.h"
#include "VertexArray.h"
#include "Shader.h"
#include <memory>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>
#include <string>
#include <map>

namespace McEngine
{
namespace Meshes
{
struct Object
{
    std::shared_ptr<VertexArray> m_vertexArray;
    std::shared_ptr<Shaders::Shader> m_shaderProgram;
    std::vector<std::string> m_textures;
    std::string m_currentAvaiableTexture;
    glm::mat4 m_modelMatrix;
    glm::vec3 m_position;
    glm::vec3 m_rotatione;
    glm::vec3 m_scale;
    std::string m_label;

    Object()
    {
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotatione = glm::vec3(0.0f, 0.0f, 0.0f);
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_modelMatrix = glm::mat4();
        m_label = "";
    }

    void reset()
    {
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotatione = glm::vec3(0.0f, 0.0f, 0.0f);
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_modelMatrix = glm::mat4();
        m_label = "";
    }

};

}//Meshes
}//McEngine

