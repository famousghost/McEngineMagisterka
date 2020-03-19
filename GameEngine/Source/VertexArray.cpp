#include "VertexArray.h"

namespace McEngine
{
namespace Meshes
{
VertexArray::~VertexArray()
{
    glDeleteBuffers(VBO_SIZE, m_vboId.data());
    glDeleteBuffers(1, &m_eboId);
    glDeleteVertexArrays(1, &m_vaoId);
}

void VertexArray::createVbo()
{
    m_vboId = std::vector<GLuint>(VBO_SIZE);
    glGenBuffers(VBO_SIZE, m_vboId.data());
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
