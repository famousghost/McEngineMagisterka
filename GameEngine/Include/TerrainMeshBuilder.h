#pragma once
#include "ModelBuilder.h"

namespace McEngine
{
namespace Meshes
{
class TerrainMeshBuilder : public ModelBuilder
{
    public:
        TerrainMeshBuilder();
        TerrainMeshBuilder(int p_columns, int p_rows);
        
        ModelBuilder& buildVerticies() override;

        std::shared_ptr<Mesh> getMesh() const override;
    private:
        std::shared_ptr<Mesh> m_mesh;
        int m_columns;
        int m_rows;
};
}//Meshes
}//McEngine

