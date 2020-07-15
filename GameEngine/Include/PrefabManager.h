#pragma once
#include "VertexArray.h"
#include "Mesh.h"
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

    void createScreenMesh();

    void createCubeColiderMesh();

    void createSkyBoxMesh();

    void createTerrainMesh();

    void prepareVertexArrayObject(std::shared_ptr<Mesh>& p_mesh);

    std::shared_ptr<Mesh> getScreenQuadMesh() const;

    std::shared_ptr<Mesh> getColiderMesh() const;

    std::shared_ptr<Mesh> getTerrainMesh() const;

    std::shared_ptr<Mesh> getSkyBoxMesh() const;

    bool loadMeshFromFile(std::string p_filePath);

    std::shared_ptr<Mesh> getMesh(std::string p_label) const;

    std::vector<std::shared_ptr<Mesh>> getMeshes(std::string p_label) const;

private:
    std::string m_directory;
    std::string m_objectName;

    std::map<std::string, std::shared_ptr<Mesh>> m_prefabMeshes;
    std::shared_ptr<Mesh> m_screenQuadMesh;
    std::shared_ptr<Mesh> m_cubeColiderMesh;
    std::shared_ptr<Mesh> m_terrainMesh;
    std::shared_ptr<Mesh> m_skyBoxMesh;

    std::vector<std::shared_ptr<Mesh>> loadMesh(std::string p_filePath);

    void processNode(aiNode * p_node, 
                     const aiScene * p_scene, 
                     std::vector<std::shared_ptr<Mesh>>& p_meshes);

    void loadTextures(std::shared_ptr<Mesh>& p_prefabMesh, aiMesh * p_mesh, const aiScene * p_scene);

    void setVertexArrayForMesh(std::shared_ptr<Mesh>& p_mesh);

    void fillMeshValues(std::shared_ptr<Mesh>& p_prefabMesh, aiMesh * p_mesh);

    void fillIndicies(std::shared_ptr<Mesh>& p_prefabMesh, aiMesh * p_mesh);

    std::shared_ptr<Mesh> processMesh(aiMesh * p_mesh, const aiScene * p_scene);

    void addMeshesToMap(std::vector<std::shared_ptr<Mesh>>& p_meshes);

    PrefabManager() = default;
    ~PrefabManager() = default;
};

}//Meshes
}//McEngine