#pragma once
#include <iostream>
#include <vector>
#include "VertexArray.h"
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Meshes
{

struct Texture
{
    uint32_t id;
    std::string label;
    std::string path;
};

struct Mesh
{
    std::vector<glm::vec3> m_vertexCoords;
    std::vector<glm::vec3> m_colorValues;
    std::vector<glm::vec3> m_normalCoords;
    std::vector<glm::vec2> m_textureCoords;
    std::vector<uint32_t> m_indicies;
    std::vector<Texture> m_textures;
    VertexArray m_vertexArray;
};

}//Meshes
}//GameEngine