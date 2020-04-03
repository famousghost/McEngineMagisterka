#include "CubeModelBuilder.h"
#include <algorithm>
#include <vector>
#include <gtest\gtest.h>
#include <gmock\gmock.h>

using testing::_;
using testing::Return;
using testing::AtLeast;
using testing::NiceMock;
using testing::StrictMock;

namespace McEngine
{
namespace Meshes
{

struct CubeModelBuilderTestSuite : public testing::Test
{
    CubeModelBuilder m_cubeBuilder;
};

TEST_F(CubeModelBuilderTestSuite, CheckIfVertexArrayIsAppropriatelyBuild)
{
    std::vector<VertexCoords> l_expectedVertCoords =
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
        VertexCoords{ 0.5f, 0.0f, -0.5f },
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
    m_cubeBuilder.buildVertexCoordinatesArray();
    auto& l_vertCoords = m_cubeBuilder.getModel()->m_vertexCoords;
    ASSERT_TRUE(l_vertCoords.size() > 0);
    ASSERT_TRUE(std::equal(l_vertCoords.begin(), l_vertCoords.end(), l_expectedVertCoords.begin()));
}

TEST_F(CubeModelBuilderTestSuite, CheckIfColorValueArrayIsAppropriatelyBuild)
{
    std::vector<ColorValues> l_expectedColorValues = 
    {
        ColorValues{ 1.0f, 0.0f, 0.0f },
        ColorValues{ 1.0f, 0.0f, 0.0f },
        ColorValues{ 1.0f, 0.0f, 0.0f },
        ColorValues{ 1.0f, 0.0f, 0.0f },
        ColorValues{ 1.0f, 0.0f, 0.0f },
        ColorValues{ 1.0f, 0.0f, 0.0f },

        ColorValues{ 0.0f, 1.0f, 0.0f },
        ColorValues{ 0.0f, 1.0f, 0.0f },
        ColorValues{ 0.0f, 1.0f, 0.0f },
        ColorValues{ 0.0f, 1.0f, 0.0f },
        ColorValues{ 0.0f, 1.0f, 0.0f },
        ColorValues{ 0.0f, 1.0f, 0.0f },

        ColorValues{ 0.0f, 0.0f, 1.0f },
        ColorValues{ 0.0f, 0.0f, 1.0f },
        ColorValues{ 0.0f, 0.0f, 1.0f },
        ColorValues{ 0.0f, 0.0f, 1.0f },
        ColorValues{ 0.0f, 0.0f, 1.0f },
        ColorValues{ 0.0f, 0.0f, 1.0f },

        ColorValues{ 1.0f, 0.0f, 1.0f },
        ColorValues{ 1.0f, 0.0f, 1.0f },
        ColorValues{ 1.0f, 0.0f, 1.0f },
        ColorValues{ 1.0f, 0.0f, 1.0f },
        ColorValues{ 1.0f, 0.0f, 1.0f },
        ColorValues{ 1.0f, 0.0f, 1.0f },

        ColorValues{ 1.0f, 1.0f, 1.0f },
        ColorValues{ 1.0f, 1.0f, 1.0f },
        ColorValues{ 1.0f, 1.0f, 1.0f },
        ColorValues{ 1.0f, 1.0f, 1.0f },
        ColorValues{ 1.0f, 1.0f, 1.0f },
        ColorValues{ 1.0f, 1.0f, 1.0f },

        ColorValues{ 0.0f, 0.0f, 0.0f },
        ColorValues{ 0.0f, 0.0f, 0.0f },
        ColorValues{ 0.0f, 0.0f, 0.0f },
        ColorValues{ 0.0f, 0.0f, 0.0f },
        ColorValues{ 0.0f, 0.0f, 0.0f },
        ColorValues{ 0.0f, 0.0f, 0.0f }
    };
    m_cubeBuilder.buildColorValuesArray();
    auto& l_colorValues = m_cubeBuilder.getModel()->m_colorValues;
    ASSERT_TRUE(l_colorValues.size() > 0);
    ASSERT_TRUE(std::equal(l_colorValues.begin(), l_colorValues.end(), l_expectedColorValues.begin()));
}

TEST_F(CubeModelBuilderTestSuite, CheckIfNormalCoordsArrayIsAppropriatelyBuild)
{
    std::vector<NormalCoords> l_expectedNormalCoords =
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
    m_cubeBuilder.buildNormalValuesArray();
    auto& l_normalCoords = m_cubeBuilder.getModel()->m_normalCoords;
    ASSERT_TRUE(l_normalCoords.size() > 0);
    ASSERT_TRUE(std::equal(l_normalCoords.begin(), l_normalCoords.end(), l_expectedNormalCoords.begin()));
}

TEST_F(CubeModelBuilderTestSuite, CheckIfTextrueCoordsArrayIsAppropriatelyBuild)
{
    std::vector<TextureCoords> l_expectedTextureCoords =
    {
        TextureCoords{ 0.0f, 0.0f },
        TextureCoords{ 1.0f, 0.0f},
        TextureCoords{ 1.0f, 1.0f  },
        TextureCoords{ 1.0f, 1.0f },
        TextureCoords{ 0.0f, 1.0f },
        TextureCoords{ 0.0f, 0.0f },

        TextureCoords{ 0.0f, 0.0f },
        TextureCoords{ 1.0f, 0.0f},
        TextureCoords{ 1.0f, 1.0f  },
        TextureCoords{ 1.0f, 1.0f },
        TextureCoords{ 0.0f, 1.0f },
        TextureCoords{ 0.0f, 0.0f },

        TextureCoords{ 0.0f, 0.0f },
        TextureCoords{ 1.0f, 0.0f},
        TextureCoords{ 1.0f, 1.0f  },
        TextureCoords{ 1.0f, 1.0f },
        TextureCoords{ 0.0f, 1.0f },
        TextureCoords{ 0.0f, 0.0f },

        TextureCoords{ 0.0f, 0.0f },
        TextureCoords{ 1.0f, 0.0f},
        TextureCoords{ 1.0f, 1.0f  },
        TextureCoords{ 1.0f, 1.0f },
        TextureCoords{ 0.0f, 1.0f },
        TextureCoords{ 0.0f, 0.0f },

        TextureCoords{ 0.0f, 0.0f },
        TextureCoords{ 1.0f, 0.0f},
        TextureCoords{ 1.0f, 1.0f  },
        TextureCoords{ 1.0f, 1.0f },
        TextureCoords{ 0.0f, 1.0f },
        TextureCoords{ 0.0f, 0.0f },

        TextureCoords{ 0.0f, 0.0f },
        TextureCoords{ 1.0f, 0.0f},
        TextureCoords{ 1.0f, 1.0f  },
        TextureCoords{ 1.0f, 1.0f },
        TextureCoords{ 0.0f, 1.0f },
        TextureCoords{ 0.0f, 0.0f }
    };
    m_cubeBuilder.buildTextureCoordinatesArray();
    auto& l_textureCoords = m_cubeBuilder.getModel()->m_textureCoords;
    ASSERT_TRUE(l_textureCoords.size() > 0);
    ASSERT_TRUE(std::equal(l_textureCoords.begin(), l_textureCoords.end(), l_expectedTextureCoords.begin()));
}

}//Meshes
}//GameEngine