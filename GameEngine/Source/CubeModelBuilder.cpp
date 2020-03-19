#include "CubeModelBuilder.h"
namespace McEngine
{
namespace Meshes
{
CubeModelBuilder::CubeModelBuilder()
    :m_model(std::make_shared<Model>())
{
}

void CubeModelBuilder::buildVertexCoordinatesArray()
{
    m_model->m_vertexCoords = std::vector<VertexCoords>
    {
        VertexCoords{ -0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, 0.5f, -0.5f },
        VertexCoords{ 0.5f, 0.5f, -0.5f },
        VertexCoords{ -0.5f, 0.5f, -0.5f },
        VertexCoords{ -0.5f, -0.5f, -0.5f },

        VertexCoords{ -0.5f, -0.5f, 0.5f },
        VertexCoords{ 0.5f, -0.5f, 0.5f },
        VertexCoords{ 0.5f, 0.5f, 0.5f },
        VertexCoords{ 0.5f, 0.5f, 0.5f },
        VertexCoords{ -0.5f, 0.5f, 0.5f },
        VertexCoords{ -0.5f, -0.5f, 0.5f },

        VertexCoords{ -0.5f, 0.5f, 0.5f },
        VertexCoords{ -0.5f, 0.5f, -0.5f },
        VertexCoords{ -0.5f, -0.5f, -0.5f },
        VertexCoords{ -0.5f, -0.5f, -0.5f },
        VertexCoords{ -0.5f, -0.5f, 0.5f },
        VertexCoords{ -0.5f, 0.5f, 0.5f },

        VertexCoords{ 0.5f, 0.5f, 0.5f },
        VertexCoords{ 0.5f, 0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, 0.5f },
        VertexCoords{ 0.5f, 0.5f, 0.5f },

        VertexCoords{ -0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, -0.5f },
        VertexCoords{ 0.5f, -0.5f, 0.5f },
        VertexCoords{ 0.5f, -0.5f, 0.5f },
        VertexCoords{ -0.5f, -0.5f, 0.5f },
        VertexCoords{ -0.5f, -0.5f, -0.5f },

        VertexCoords{ -0.5f, 0.5f, -0.5f },
        VertexCoords{ 0.5f, 0.5f, -0.5f },
        VertexCoords{ 0.5f, 0.5f, 0.5f },
        VertexCoords{ 0.5f, 0.5f, 0.5f },
        VertexCoords{ -0.5f, 0.5f, 0.5f },
        VertexCoords{ -0.5f, 0.5f, -0.5f }
    };
}

void CubeModelBuilder::buildColorValuesArray(const std::vector<ColorValues>& p_colorValues)
{
    if(p_colorValues.size() == 36)
    {
        m_model->m_colorValues = p_colorValues;
    }
}

void CubeModelBuilder::buildTextureCoordinatesArray(const std::vector<TextureCoords>& p_textureCoords)
{
    if(p_textureCoords.size() == 36)
    {
        m_model->m_textureCoords = p_textureCoords;
    }
}

void CubeModelBuilder::buildNormalValuesArray()
{
    m_model->m_normalCoords = std::vector<NormalCoords>
    {
        NormalCoords{ 0.0f, 0.0f, -1.0f },
        NormalCoords{ 0.0f, 0.0f, -1.0f },
        NormalCoords{ 0.0f, 0.0f, -1.0f },
        NormalCoords{ 0.0f, 0.0f, -1.0f },
        NormalCoords{ 0.0f, 0.0f, -1.0f },
        NormalCoords{ 0.0f, 0.0f, -1.0f },

        NormalCoords{ 0.0f, 0.0f, 1.0f },
        NormalCoords{ 0.0f, 0.0f, 1.0f },
        NormalCoords{ 0.0f, 0.0f, 1.0f },
        NormalCoords{ 0.0f, 0.0f, 1.0f },
        NormalCoords{ 0.0f, 0.0f, 1.0f },
        NormalCoords{ 0.0f, 0.0f, 1.0f },

        NormalCoords{ -1.0f, 0.0f, 0.0f },
        NormalCoords{ -1.0f, 0.0f, 0.0f },
        NormalCoords{ -1.0f, 0.0f, 0.0f },
        NormalCoords{ -1.0f, 0.0f, 0.0f },
        NormalCoords{ -1.0f, 0.0f, 0.0f },
        NormalCoords{ -1.0f, 0.0f, 0.0f },

        NormalCoords{ 1.0f, 0.0f, 0.0f },
        NormalCoords{ 1.0f, 0.0f, 0.0f },
        NormalCoords{ 1.0f, 0.0f, 0.0f },
        NormalCoords{ 1.0f, 0.0f, 0.0f },
        NormalCoords{ 1.0f, 0.0f, 0.0f },
        NormalCoords{ 1.0f, 0.0f, 0.0f },

        NormalCoords{ 0.0f, -1.0f, 0.0f },
        NormalCoords{ 0.0f, -1.0f, 0.0f },
        NormalCoords{ 0.0f, -1.0f, 0.0f },
        NormalCoords{ 0.0f, -1.0f, 0.0f },
        NormalCoords{ 0.0f, -1.0f, 0.0f },
        NormalCoords{ 0.0f, -1.0f, 0.0f },

        NormalCoords{ 0.0f, 1.0f, 0.0f },
        NormalCoords{ 0.0f, 1.0f, 0.0f },
        NormalCoords{ 0.0f, 1.0f, 0.0f },
        NormalCoords{ 0.0f, 1.0f, 0.0f },
        NormalCoords{ 0.0f, 1.0f, 0.0f },
        NormalCoords{ 0.0f, 1.0f, 0.0f }
    };
}

void CubeModelBuilder::reset()
{
    m_model.reset(new Model);
}

std::shared_ptr<Model> CubeModelBuilder::getModel() const
{
    return m_model;
}

}//Meshes
}//GameEngine