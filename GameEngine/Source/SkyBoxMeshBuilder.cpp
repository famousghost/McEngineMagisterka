#include "SkyBoxMeshBuilder.h"

namespace McEngine
{
namespace Meshes
{

ModelBuilder & SkyBoxMeshBuilder::buildVerticies()
{
    m_mesh = std::make_shared<Mesh>();
    std::vector<glm::vec3> l_verticies =
    { 
        glm::vec3{ -1.0f, -1.0f, -1.0f },
        glm::vec3{ 1.0f, -1.0f, -1.0f },
        glm::vec3{ 1.0f, 1.0f, -1.0f },
        glm::vec3{ -1.0f, 1.0f, -1.0f },

        glm::vec3{ -1.0f, -1.0f, 1.0f },
        glm::vec3{ 1.0f, -1.0f, 1.0f },
        glm::vec3{ 1.0f, 1.0f, 1.0f },
        glm::vec3{ -1.0f, 1.0f, 1.0f },

        glm::vec3{ -1.0f, 1.0f, 1.0f },
        glm::vec3{ -1.0f, 1.0f, -1.0f },
        glm::vec3{ -1.0f, -1.0f, -1.0f },
        glm::vec3{ -1.0f, -1.0f, 1.0f },

        glm::vec3{ 1.0f, 1.0f, 1.0f },
        glm::vec3{ 1.0f, 1.0f, -1.0f },
        glm::vec3{ 1.0f, -1.0f, -1.0f },
        glm::vec3{ 1.0f, -1.0f, 1.0f },

        glm::vec3{ -1.0f, -1.0f, -1.0f },
        glm::vec3{ 1.0f, -1.0f, -1.0f },
        glm::vec3{ 1.0f, -1.0f, 1.0f },
        glm::vec3{ -1.0f, -1.0f, 1.0f },

        glm::vec3{ -1.0f, 1.0f, -1.0f },
        glm::vec3{ 1.0f, 1.0f, -1.0f },
        glm::vec3{ 1.0f, 1.0f, 1.0f },
        glm::vec3{ -1.0f, 1.0f, 1.0f }
    };

    std::vector<glm::vec3> l_normals =
    {
        glm::vec3{ 0.0f,  0.0f, -1.0f },
        glm::vec3{ 0.0f,  0.0f, -1.0f },
        glm::vec3{ 0.0f,  0.0f, -1.0f },
        glm::vec3{ 0.0f,  0.0f, -1.0f },

        glm::vec3{ 0.0f,  0.0f, 1.0f },
        glm::vec3{ 0.0f,  0.0f, 1.0f },
        glm::vec3{ 0.0f,  0.0f, 1.0f },
        glm::vec3{ 0.0f,  0.0f, 1.0f },

        glm::vec3{ -1.0f,  0.0f, 0.0f },
        glm::vec3 { -1.0f,  0.0f, 0.0f },
        glm::vec3{ -1.0f,  0.0f, 0.0f },
        glm::vec3{ -1.0f,  0.0f, 0.0f },

        glm::vec3{ 1.0f,  0.0f, 0.0f },
        glm::vec3 { 1.0f,  0.0f, 0.0f },
        glm::vec3{ 1.0f,  0.0f, 0.0f },
        glm::vec3{ 1.0f,  0.0f, 0.0f },

        glm::vec3{ 0.0f,  -1.0f, 0.0f },
        glm::vec3{ 0.0f,  -1.0f, 0.0f },
        glm::vec3{ 0.0f,  -1.0f, 0.0f },
        glm::vec3{ 0.0f,  -1.0f, 0.0f },

        glm::vec3{ 0.0f,  1.0f, 0.0f },
        glm::vec3{ 0.0f,  1.0f, 0.0f },
        glm::vec3{ 0.0f,  1.0f, 0.0f },
        glm::vec3{ 0.0f,  1.0f, 0.0f },
    };

    std::vector<glm::vec2> l_textureCoords =
    {
        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 1.0f, 0.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ 0.0f, 1.0f },
        glm::vec2{ 0.0f, 0.0f },

        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 1.0f, 0.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ 0.0f, 1.0f },
        glm::vec2{ 0.0f, 0.0f },

        glm::vec2{ 1.0f, 0.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ 0.0f, 1.0f },
        glm::vec2{ 0.0f, 1.0f },
        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 1.0f, 0.0f },

        glm::vec2{ 1.0f, 0.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ 0.0f, 1.0f },
        glm::vec2{ 0.0f, 1.0f },
        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 1.0f, 0.0f },

        glm::vec2{ 0.0f, 1.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ 1.0f, 0.0f },
        glm::vec2{ 1.0f, 0.0f },
        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 0.0f, 1.0f },

        glm::vec2{ 0.0f, 1.0f },
        glm::vec2{ 1.0f, 1.0f },
        glm::vec2{ 1.0f, 0.0f },
        glm::vec2{ 1.0f, 0.0f },
        glm::vec2{ 0.0f, 0.0f },
        glm::vec2{ 0.0f, 1.0f }
    };

    std::vector<GLuint> l_indicies
    {
        0, 1, 2,
        2, 3, 0, // Back
        6, 5, 4,
        4, 7, 6, // Front
        11, 10, 9,
        9, 8, 11, // Left
        14, 15, 12,
        12, 13, 14, // Right
        19, 18, 17,
        17, 16, 19, //Down
        20, 21, 22,
        22, 23, 20 // Up
    };

    for (std::size_t i = 0; i < l_verticies.size(); ++i)
    {
        Vertex l_vertex;
        l_vertex.m_position = l_verticies[i];
        l_vertex.m_normalCoords = l_normals[i];
        l_vertex.m_textureCoords = l_textureCoords[i];
        m_mesh->m_verticies.push_back(l_vertex);
    }

    for (std::size_t i = 0; i < l_indicies.size(); ++i)
    {
        m_mesh->m_indicies.push_back(l_indicies[i]);
    }

    return *this;
}

std::shared_ptr<Mesh> SkyBoxMeshBuilder::getMesh() const
{
    return m_mesh;
}

}//Meshes
}//McEngine
