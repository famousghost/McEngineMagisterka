#include "CubeColiderBuilder.h"

namespace McEngine
{
namespace Meshes
{

CubeColiderBuilder::CubeColiderBuilder()
{
    m_mesh = std::make_shared<Mesh>();
}

void CubeColiderBuilder::buildVerticies()
{
    std::vector<glm::vec3> l_verticies =
    {
        glm::vec3{-1.0f, 1.0f, 1.0f},
        glm::vec3{-1.0f, -1.0f, 1.0f},
        glm::vec3{1.0f, -1.0f, 1.0f},
        glm::vec3{1.0f, 1.0f, 1.0f},
        glm::vec3{-1.0f, 1.0f, -1.0f},
        glm::vec3{-1.0f, -1.0f, -1.0f},
        glm::vec3{1.0f, -1.0f, -1.0f},
        glm::vec3{1.0f, 1.0f, -1.0f}
    };

    std::vector<GLuint> l_indicies = 
    {
        0, 1, 2,
        0, 3, 2,// Front
        4, 5, 1,
        4, 0, 1,// Left
        4, 5, 6,
        4, 7, 6,// Back
        7, 6, 2,
        7, 3, 2,//Prawa
        0, 4, 7,
        0, 3, 7, //Gora
        1, 5, 6,
        1, 2, 6 // Dó³
    };

    for (std::size_t i = 0; i < l_verticies.size(); ++i)
    {
        Vertex l_vertex;
        l_vertex.m_position = l_verticies[i];
        l_vertex.m_normalCoords = glm::vec3(0.0f, 0.0f, 0.0f);
        l_vertex.m_textureCoords = glm::vec2(0.0f, 0.0f);
        m_mesh->m_verticies.push_back(l_vertex);
    }

    for (std::size_t i = 0; i < l_indicies.size(); ++i)
    {
        m_mesh->m_indicies.push_back(l_indicies[i]);
    }
}

std::shared_ptr<Mesh> CubeColiderBuilder::getMesh() const
{
    return m_mesh;
}

}//Meshes
}//McEngine