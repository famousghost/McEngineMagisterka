#pragma once
#include <iostream>
#include <vector>
#include "VertexArray.h"

namespace McEngine
{
namespace Meshes
{

struct Texture
{
    unsigned int id;
    std::string label;
    std::string path;
};

struct Mesh
{
    std::vector<Vertex> m_verticies;
    std::vector<unsigned int> m_indicies;
    std::vector<Texture> m_textures;
    VertexArray m_vertexArray;
};

}//Meshes
}//GameEngine