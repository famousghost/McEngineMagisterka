#pragma once
#include "ModelBuilder.h"

namespace McEngine
{
namespace Meshes
{
class ScreenQuadMeshBuilder : public ModelBuilder
{
public:
    ModelBuilder& buildVerticies() override;

    std::shared_ptr<Mesh> getMesh() const override;

private:
    std::shared_ptr<Mesh> m_mesh;
};

}//Meshes
}//McEngine