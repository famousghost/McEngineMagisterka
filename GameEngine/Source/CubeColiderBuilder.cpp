#include "CubeColiderBuilder.h"

namespace McEngine
{
namespace Meshes
{

CubeColiderBuilder::CubeColiderBuilder()
{
    m_mesh = std::make_shared<Mesh>();
}

ModelBuilder& CubeColiderBuilder::buildVerticies()
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
        3, 0, 2,// Front

        4, 5, 6,
        7, 6, 4,// Back

        5, 4, 6,
        0, 4, 1,// Left

        3, 2, 6,
        7, 3, 6,//Prawa

        7, 3, 6,
        4, 3, 0, //Gora

        1, 5, 6,
        2, 1, 6 // Dó³
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
    return *this;
}

std::shared_ptr<Mesh> CubeColiderBuilder::getMesh() const
{
    return m_mesh;
}

}//Meshes
}//McEngine