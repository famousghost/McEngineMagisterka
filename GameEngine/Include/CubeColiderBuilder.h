#pragma once
#include "ModelBuilder.h"

namespace McEngine
{
namespace Meshes
{

class CubeColiderBuilder : public ModelBuilder
{
public:
    CubeColiderBuilder();
    void buildVerticies() override;

    std::shared_ptr<Mesh> getMesh() const override;
private:
    std::shared_ptr<Mesh> m_mesh;
};

}//Meshes
}//McEngine
