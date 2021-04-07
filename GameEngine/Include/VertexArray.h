#pragma once
#include <glad.h>
#include <vector>
#include "Logger.h"
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Meshes
{
struct Vertex
{
    glm::vec3 m_position;
    glm::vec3 m_normalCoords;
    glm::vec3 m_tangent;
    glm::vec2 m_textureCoords;
};

class VertexArray
{
public:
    

    VertexArray();
    ~VertexArray();

    void createVbo();

    void addValuesToAttribPointer(const std::vector<Vertex>& p_values);

    void addIndicies(const std::vector<unsigned int>& p_indicies);

    template<typename Type>
    void bufferDataT(GLenum p_target, const std::vector<Type>& p_array, GLenum p_usage)
    {
        glBufferData(p_target, p_array.size() * sizeof(Type), &p_array[0], p_usage);
    }

    void createVao();
    void createEbo();
    void bindVao();
    void unbindVao();

private:
    GLuint m_vaoId;
    GLuint m_vboId;
    GLuint m_eboId;
};

}//Meshes
}//GameEngine
