#pragma once
#include "Model.h"
#include "VertexArray.h"
#include "Shader.h"
#include <memory>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>
#include <string>

namespace McEngine
{
namespace Meshes
{
struct Object
{
    std::shared_ptr<Model> m_model;
    std::shared_ptr<VertexArray> m_vertexArray;
    std::shared_ptr<Shaders::Shader> m_shaderProgram;
    glm::mat4 l_model;
    glm::vec3 m_position;
    glm::vec3 m_rotatione;
    glm::vec3 m_scale;
    std::string m_label;

    Object()
    {
        m_model = std::make_shared<Model>();
        m_vertexArray = std::make_shared<VertexArray>();
        m_shaderProgram = std::make_shared<Shaders::Shader>();
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotatione = glm::vec3(0.0f, 0.0f, 0.0f);
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_label = "";
    }

    void reset()
    {
        m_model.reset(new Model);
        m_vertexArray.reset(new VertexArray);
        m_shaderProgram.reset(new Shaders::Shader);
        m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotatione = glm::vec3(0.0f, 0.0f, 0.0f);
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }

};

}//Meshes
}//McEngine

