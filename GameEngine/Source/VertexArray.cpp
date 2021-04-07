#include "VertexArray.h"


namespace McEngine
{
namespace Meshes
{

VertexArray::VertexArray()
{
    createVao();
    createVbo();
    createEbo();
}

VertexArray::~VertexArray()
{
    glDeleteBuffers(1, &m_vboId);
    glDeleteBuffers(1, &m_eboId);
    glDeleteVertexArrays(1, &m_vaoId);
}

void VertexArray::createVbo()
{
    glGenBuffers(1, &m_vboId);
}

void VertexArray::addValuesToAttribPointer(const std::vector<Vertex>& p_values)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    
    bufferDataT(GL_ARRAY_BUFFER, p_values, GL_STATIC_DRAW);

    GLuint l_offset = 0;
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(l_offset));
    
    l_offset += sizeof(glm::vec3);
    
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(l_offset));

    l_offset += sizeof(glm::vec3);

    glEnableVertexAttribArray(1);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(l_offset));

    l_offset += sizeof(glm::vec3);

    glEnableVertexAttribArray(3);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(l_offset));

    l_offset += sizeof(glm::vec2);

    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::addIndicies(const std::vector<unsigned int>& p_indicies)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);
    bufferDataT(GL_ELEMENT_ARRAY_BUFFER, p_indicies, GL_STATIC_DRAW);
}

void VertexArray::createVao()
{
    glGenVertexArrays(1, &m_vaoId);
}

void VertexArray::bindVao()
{
    glBindVertexArray(m_vaoId);
}

void VertexArray::unbindVao()
{
    glBindVertexArray(0);
}

void VertexArray::createEbo()
{
    glGenBuffers(1, &m_eboId);
}

}//Meshes
}//GameEngine
