#include "CubeModelBuilder.h"

namespace McEngine
{
namespace Meshes
{
CubeModelBuilder::CubeModelBuilder()
    :m_model(std::make_shared<Mesh>())
{
}

void CubeModelBuilder::buildVertexCoordinatesArray()
{
    m_model->m_vertexCoords = std::vector<VertexCoords>
    {
        VertexCoords{ -0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, 0.5f, -0.5f },
        VertexCoords{ -0.5f, 0.5f, -0.5f },

        VertexCoords{ -0.5f, -0.5f, 0.5f },
        VertexCoords{ 0.5f, -0.5f, 0.5f },
        VertexCoords{ 0.5f, 0.5f, 0.5f },
        VertexCoords{ -0.5f, 0.5f, 0.5f },

        VertexCoords{ -0.5f, 0.5f, 0.5f },
        VertexCoords{ -0.5f, 0.5f, -0.5f },
        VertexCoords{ -0.5f, -0.5f, -0.5f },
        VertexCoords{ -0.5f, -0.5f, 0.5f },

        VertexCoords{ 0.5f, 0.5f, 0.5f },
        VertexCoords{ 0.5f, 0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, 0.5f },

        VertexCoords{ -0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, 0.5f },
        VertexCoords{ -0.5f, -0.5f, 0.5f },

        VertexCoords{ -0.5f, 0.5f, -0.5f },
        VertexCoords{ 0.5f, 0.5f, -0.5f },
        VertexCoords{ 0.5f, 0.5f, 0.5f },
        VertexCoords{ -0.5f, 0.5f, 0.5f },
    };
}

void CubeModelBuilder::buildColorValuesArray()
{
    m_model->m_colorValues = std::vector<Meshes::ColorValues>
    {
        Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },
        Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },
        Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },
        Meshes::ColorValues{ 1.0f, 0.0f, 0.0f },

        Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },
        Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },
        Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },
        Meshes::ColorValues{ 0.0f, 1.0f, 0.0f },

        Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },
        Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },
        Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },
        Meshes::ColorValues{ 0.0f, 0.0f, 1.0f },

        Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },
        Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },
        Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },
        Meshes::ColorValues{ 1.0f, 0.0f, 1.0f },

        Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },
        Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },
        Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },
        Meshes::ColorValues{ 1.0f, 1.0f, 1.0f },

        Meshes::ColorValues{ 0.0f, 0.0f, 0.0f },
        Meshes::ColorValues{ 0.0f, 0.0f, 0.0f },
        Meshes::ColorValues{ 0.0f, 0.0f, 0.0f },
        Meshes::ColorValues{ 0.0f, 0.0f, 0.0f },
    };
}

void CubeModelBuilder::buildTextureCoordinatesArray()
{
    m_model->m_textureCoords = std::vector<Meshes::TextureCoords>
    {
        Meshes::TextureCoords{ 0.0f, 0.0f },
        Meshes::TextureCoords{ 1.0f, 0.0f},
        Meshes::TextureCoords{ 1.0f, 1.0f  },
        Meshes::TextureCoords{ 0.0f, 1.0f },

        Meshes::TextureCoords{ 0.0f, 0.0f },
        Meshes::TextureCoords{ 1.0f, 0.0f},
        Meshes::TextureCoords{ 1.0f, 1.0f  },
        Meshes::TextureCoords{ 0.0f, 1.0f },

        Meshes::TextureCoords{ 0.0f, 0.0f },
        Meshes::TextureCoords{ 1.0f, 0.0f},
        Meshes::TextureCoords{ 1.0f, 1.0f },
        Meshes::TextureCoords{ 0.0f, 1.0f },

        Meshes::TextureCoords{ 0.0f, 0.0f },
        Meshes::TextureCoords{ 1.0f, 0.0f},
        Meshes::TextureCoords{ 1.0f, 1.0f  },
        Meshes::TextureCoords{ 0.0f, 1.0f },

        Meshes::TextureCoords{ 0.0f, 0.0f },
        Meshes::TextureCoords{ 1.0f, 0.0f},
        Meshes::TextureCoords{ 1.0f, 1.0f },
        Meshes::TextureCoords{ 0.0f, 1.0f },

        Meshes::TextureCoords{ 0.0f, 0.0f },
        Meshes::TextureCoords{ 1.0f, 0.0f},
        Meshes::TextureCoords{ 1.0f, 1.0f },
        Meshes::TextureCoords{ 0.0f, 1.0f },
    };
}

void CubeModelBuilder::buildNormalValuesArray()
{
    m_model->m_normalCoords = std::vector<NormalCoords>
    {
        NormalCoords{ 0.0f, 0.0f, -1.0f },
        NormalCoords{ 0.0f, 0.0f, -1.0f },
        NormalCoords{ 0.0f, 0.0f, -1.0f },
        NormalCoords{ 0.0f, 0.0f, -1.0f },

        NormalCoords{ 0.0f, 0.0f, 1.0f },
        NormalCoords{ 0.0f, 0.0f, 1.0f },
        NormalCoords{ 0.0f, 0.0f, 1.0f },
        NormalCoords{ 0.0f, 0.0f, 1.0f },

        NormalCoords{ -1.0f, 0.0f, 0.0f },
        NormalCoords{ -1.0f, 0.0f, 0.0f },
        NormalCoords{ -1.0f, 0.0f, 0.0f },
        NormalCoords{ -1.0f, 0.0f, 0.0f },

        NormalCoords{ 1.0f, 0.0f, 0.0f },
        NormalCoords{ 1.0f, 0.0f, 0.0f },
        NormalCoords{ 1.0f, 0.0f, 0.0f },
        NormalCoords{ 1.0f, 0.0f, 0.0f },

        NormalCoords{ 0.0f, -1.0f, 0.0f },
        NormalCoords{ 0.0f, -1.0f, 0.0f },
        NormalCoords{ 0.0f, -1.0f, 0.0f },
        NormalCoords{ 0.0f, -1.0f, 0.0f },

        NormalCoords{ 0.0f, 1.0f, 0.0f },
        NormalCoords{ 0.0f, 1.0f, 0.0f },
        NormalCoords{ 0.0f, 1.0f, 0.0f },
        NormalCoords{ 0.0f, 1.0f, 0.0f },
    };
}

void CubeModelBuilder::buildIndicies()
{
    m_model->m_indicies = std::vector<uint32_t>
    {
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        16, 17, 19,
        17, 18, 19,
        20, 21, 23,
        21, 22, 23
    };
}

void CubeModelBuilder::reset()
{
    m_model.reset(new Mesh);
}

std::shared_ptr<Mesh> CubeModelBuilder::getMesh() const
{
    return m_model;
}

}//Meshes
}//GameEngine