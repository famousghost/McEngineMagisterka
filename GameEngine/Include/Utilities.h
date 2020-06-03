#pragma once
#include "Shader.h"
#include <memory>
#include <list>
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
    glm::vec3 m_highlightColor;
};

struct Section
{
    glm::vec4 min;
    glm::vec4 max;
};

struct Normals
{
    glm::vec3 x;
    glm::vec3 y;
    glm::vec3 z;
};
}//Meshes
}//McEngine
