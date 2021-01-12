#include "TerrainMeshBuilder.h"
#include <random>

namespace McEngine
{
namespace Meshes
{
    TerrainMeshBuilder::TerrainMeshBuilder()
        :m_columns(20), m_rows(20)
    {
    }
    TerrainMeshBuilder::TerrainMeshBuilder(int p_columns, int p_rows)
        :m_columns(p_columns), m_rows(p_rows)
    {
    }

    ModelBuilder & TerrainMeshBuilder::buildVerticies()
    {
        m_mesh = std::make_shared<Mesh>();
        std::vector<glm::vec3> l_verticies;

        float x = -1.0f;
        float rowOffset = 1.0f / m_rows;
        float columnOffset = 1.0f / m_columns;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> distrib(0, 1);

        for (int i = 0; i < m_rows; ++i)
        {
            float z = -1.0f;
            for (int j = 0; j < m_columns; ++j)
            {
                if (i == m_rows / 2 and j == m_columns / 2)
                {
                    m_center = glm::vec3(x, 0.0f, z);
                }
                l_verticies.emplace_back(x, 0.0f, z);
                z += columnOffset;
            }
            x+= rowOffset;
        }


        std::vector<glm::vec2> l_textureCoords;
        x = -1.0f;
        for (int i = 0; i < m_rows; ++i)
        {
            float y = -1.0f;
            for (int j = 0; j < m_columns; ++j)
            {
                l_textureCoords.emplace_back(x, y);
                y += columnOffset;
            }
            x += rowOffset;
        }

        std::vector<GLuint> l_indicies;

        int row = 0;
        int col = 0;

        for (int i = 0; i < m_rows - 1; i++)
        {
            for (int j = 0; j < m_columns - 1; j++)
            {
                l_indicies.push_back(row + col);
                l_indicies.push_back(row + col + 1);
                l_indicies.push_back(row + 1 + col + m_columns);
                l_indicies.push_back(row + 1 + col + m_columns);
                l_indicies.push_back(row + col + m_columns);
                l_indicies.push_back(row + col);
                col++;
            }
            row++;
        }

        for (std::size_t i = 0; i < l_verticies.size(); ++i)
        {
            Vertex l_vertex;
            l_vertex.m_position = l_verticies.at(i);
            l_vertex.m_normalCoords = glm::vec3(0.0f, 1.0f, 0.0f);
            l_vertex.m_textureCoords = l_textureCoords.at(i);
            m_mesh->m_verticies.push_back(l_vertex);
        }

        for (std::size_t i = 0; i < l_indicies.size(); ++i)
        {
            m_mesh->m_indicies.push_back(l_indicies[i]);
        }

        return *this;
    }

    std::shared_ptr<Mesh> TerrainMeshBuilder::getMesh() const
    {
        return m_mesh;
    }
    glm::vec3 TerrainMeshBuilder::getTerrainCenter() const
    {
        return m_center;
    }
}//Meshes
}//McEngine