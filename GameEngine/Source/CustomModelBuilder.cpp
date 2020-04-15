#include "CustomModelBuilder.h"
#include "Logger.h"


namespace McEngine
{
namespace Meshes
{
CustomModelBuilder::CustomModelBuilder(const std::string& p_path)
{
    Assimp::Importer l_import;
    m_asimpScene = l_import.ReadFile(p_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!m_asimpScene || m_asimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_asimpScene->mRootNode)
    {
        std::string importErr = l_import.GetErrorString();
        std::string errMsg = "ERROR::ASSIMP::" + importErr;
        LOG(errMsg, LogType::ERR);
    }
    m_directory = p_path.substr(0, p_path.find_last_of('/'));
    LOG(m_directory, LogType::INF);
}

void CustomModelBuilder::buildVertexCoordinatesArray()
{
}

void CustomModelBuilder::buildColorValuesArray()
{
}

void CustomModelBuilder::buildTextureCoordinatesArray()
{
}

void CustomModelBuilder::buildNormalValuesArray()
{
}

void CustomModelBuilder::buildIndicies()
{
}

void CustomModelBuilder::reset()
{
}

std::shared_ptr<Model> CustomModelBuilder::getModel() const
{
    return std::shared_ptr<Model>();
}

}//Meshes
}//McEngine