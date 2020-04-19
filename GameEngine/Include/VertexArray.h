#pragma once
#include <glad.h>
#include <vector>
#include "Logger.h"
namespace McEngine
{
namespace Meshes
{
constexpr std::size_t VBO_SIZE = 4;
class VertexArray
{
public:
    

    VertexArray() = default;
    ~VertexArray();

    void createVbo();
    template<typename ValuesArray>
    void addValuesToAttribPointer(int p_index, const std::vector<ValuesArray>& p_values)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vboId.at(p_index));

        bufferDataT(GL_ARRAY_BUFFER, p_values, GL_STATIC_DRAW);

        glVertexAttribPointer(p_index, sizeof(ValuesArray)/sizeof(p_values[0].x), GL_FLOAT, GL_FALSE, sizeof(ValuesArray), (GLvoid*)0);

        glEnableVertexAttribArray(p_index);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void addIndicies(const std::vector<uint32_t>& p_indicies)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);

        bufferDataT(GL_ELEMENT_ARRAY_BUFFER, p_indicies, GL_STATIC_DRAW);
    }

    void createVao();
    void bindVao();
    void unbindVao();
    void createEbo();

    template<typename Type>
    void bufferDataT(GLenum p_target, const std::vector<Type>& p_array, GLenum p_usage)
    {
        glBufferData(p_target, p_array.size() * sizeof(Type), &p_array[0], p_usage);
    }

private:
    GLuint m_vaoId;
    std::vector<GLuint> m_vboId;
    GLuint m_eboId;
};

}//Meshes
}//GameEngine
