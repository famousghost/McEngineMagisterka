#pragma once
#include "ModelBuilder.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace McEngine
{
namespace Meshes
{
class CustomModelBuilder : public ModelBuilder
{
public:
    CustomModelBuilder(const std::string& p_path);
    void buildVertexCoordinatesArray() override;
    void buildColorValuesArray() override;
    void buildTextureCoordinatesArray() override;
    void buildNormalValuesArray() override;
    void buildIndicies() override;
    void reset() override;

    std::shared_ptr<Model> getModel() const override;
private:
    std::shared_ptr<Model> m_model;
    const aiScene *m_asimpScene;
    std::string m_directory;
};

}//Meshes
}//McEngine
