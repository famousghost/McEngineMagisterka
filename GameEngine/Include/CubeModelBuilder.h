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
    void buildColorValuesArray() override;
    void buildTextureCoordinatesArray() override;
    void buildNormalValuesArray() override;
    void buildIndicies() override;
    void reset() override;

    std::shared_ptr<Model> getModel() const override;
private:
    std::shared_ptr<Model> m_model;
};

}//Meshes
}//GameEngine