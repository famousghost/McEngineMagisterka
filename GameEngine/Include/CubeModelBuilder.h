#pragma once
#include "ModelBuilder.h"
namespace McEngine
{
namespace Meshes
{
class CubeModelBuilder : public ModelBuilder
{
public:
    CubeModelBuilder();
    void buildVertexCoordinatesArray() override;
    void buildColorValuesArray(const std::vector<ColorValues>& p_colorValues) override;
    void buildTextureCoordinatesArray(const std::vector<TextureCoords>& p_textureCoords) override;
    void buildNormalValuesArray() override;
    void reset() override;

    std::shared_ptr<Model> getModel() const override;
private:
    std::shared_ptr<Model> m_model;
};

}//Meshes
}//GameEngine