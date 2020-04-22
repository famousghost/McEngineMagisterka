#pragma once
#include "VertexArray.h"
#include "CubeModelBuilder.h"
#include <map>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

    std::vector<std::shared_ptr<Mesh>> getMeshes(std::string p_label) const;

private:
    std::string m_directory;
    std::string m_objectName;

    std::map<std::string, std::shared_ptr<Mesh>> m_prefabMeshes;

    std::shared_ptr<Mesh> createDefaultMesh(std::string p_label);

    std::vector<std::shared_ptr<Mesh>> loadMesh(std::string p_filePath);

    void processNode(aiNode * p_node, 
                     const aiScene * p_scene, 
                     std::vector<std::shared_ptr<Mesh>>& p_meshes);

    std::shared_ptr<Mesh> processMesh(aiMesh * p_mesh, const aiScene * p_scene);

    void createCube(std::unique_ptr<ModelBuilder>& p_modelBuilder);

    PrefabManager() = default;
    ~PrefabManager() = default;
};

}//Meshes
}//McEngine