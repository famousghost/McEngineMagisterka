#include "ScreenQuadMeshBuilder.h"

namespace McEngine
{
namespace Meshes
{

ModelBuilder& ScreenQuadMeshBuilder::buildVerticies()
{
    m_mesh = std::make_shared<Mesh>();
    std::vector<glm::vec3> l_verticies = std::vector<glm::vec3>
    {
        glm::vec3(-0.99f,  0.99f, 0.0f),
        glm::vec3(-0.99f, 0.01f, 0.0f),
        glm::vec3(0.0f, 0.01f, 0.0f),
        glm::vec3(-0.99f,  0.99f, 0.0f),
        glm::vec3(0.0f, 0.01f, 0.0f),
        glm::vec3(0.0f,  0.99f, 0.0f)
    };


    std::vector<glm::vec2> l_textureCoords = std::vector<glm::vec2>
    {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f)
    };

    for (std::size_t i = 0; i < l_verticies.size(); ++i)
    {
        Vertex l_vertex;
        l_vertex.m_position = l_verticies.at(i);
        l_vertex.m_normalCoords = glm::vec3(0.0f, 0.0f, 0.0f);
        l_vertex.m_textureCoords = l_textureCoords.at(i);
        m_mesh->m_verticies.push_back(l_vertex);
    }

    return *this;
}

std::shared_ptr<Mesh> ScreenQuadMeshBuilder::getMesh() const
{
    return m_mesh;
}

}//Meshes
}//McEngine