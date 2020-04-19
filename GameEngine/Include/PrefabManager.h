#pragma once
#include "VertexArray.h"
#include "CubeModelBuilder.h"
#include <map>
#include <string>

namespace McEngine
{
namespace Meshes
{
class PrefabManager
{
public:
    static PrefabManager& getInstance();

    void start();

    void shutdown();

    void addDefaultMesh();

    void loadMeshFromFile(std::string p_pathFile);

    std::shared_ptr<Mesh> getMesh(std::string p_label) const;

private:
    std::string m_directory;

    std::map<std::string, std::shared_ptr<Mesh>> m_prefabMeshes;

    std::shared_ptr<Mesh> createDefaultMesh(std::string p_label);

    std::shared_ptr<Mesh> loadMesh(std::string p_filePath);

    void createCube(std::unique_ptr<ModelBuilder>& p_modelBuilder);

    PrefabManager() = default;
    ~PrefabManager() = default;
};

}//Meshes
}//McEngine