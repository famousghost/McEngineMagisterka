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

        glm::vec3 getTerrainCenter() const;
    private:
        std::shared_ptr<Mesh> m_mesh;
        glm::vec3 m_center;
        int m_columns;
        int m_rows;
};
}//Meshes
}//McEngine

