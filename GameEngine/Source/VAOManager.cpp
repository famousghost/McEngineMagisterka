#include "VAOManager.h"
#include "CubeModelBuilder.h"

namespace McEngine
{
namespace Meshes
{

VAOManager& VAOManager::getInstance()
{
    static VAOManager vaoManager;
    return vaoManager;
}

void VAOManager::start()
{
    addDefaultVertexArrayObjects();
}

void VAOManager::shutdown()
{
    m_vertexArray.clear();
}

void VAOManager::addDefaultVertexArrayObjects()
{
    std::string l_cube = "CUBE";
    m_vertexArray.insert(std::make_pair(l_cube, createDefaultVertexArrayObject(l_cube)));
}

std::shared_ptr<VertexArray> VAOManager::getVertexArray(std::string p_label) const
{
    return m_vertexArray.at(p_label);
}

std::shared_ptr<VertexArray> VAOManager::createDefaultVertexArrayObject(std::string p_label)
{
    std::shared_ptr<VertexArray> l_vertexArray = std::make_shared<VertexArray>();
    std::unique_ptr<ModelBuilder> l_modelBuilder;
    if(p_label == "CUBE")
    {
        l_modelBuilder = std::make_unique<CubeModelBuilder>();
        l_modelBuilder->buildVertexCoordinatesArray();
        auto l_vertexCoordSize = l_modelBuilder->getModel()->m_vertexCoords.size();

        l_modelBuilder->buildColorValuesArray();
        auto l_colorValueSize = l_modelBuilder->getModel()->m_colorValues.size();

        l_modelBuilder->buildTextureCoordinatesArray();
        auto l_textureCoordsSize = l_modelBuilder->getModel()->m_textureCoords.size();

        l_modelBuilder->buildNormalValuesArray();
        auto l_normalCoordsSize = l_modelBuilder->getModel()->m_normalCoords.size();

        l_modelBuilder->buildIndicies();

        l_vertexArray->createVao();
        l_vertexArray->createVbo();
        l_vertexArray->createEbo();


        l_vertexArray->bindVao();
        if(l_vertexCoordSize)
        {
            l_vertexArray->addValuesToAttribPointer(0, l_modelBuilder->getModel()->m_vertexCoords);
        }

        if(l_colorValueSize)
        {
            l_vertexArray->addValuesToAttribPointer(1, l_modelBuilder->getModel()->m_colorValues);
        }

        if(l_textureCoordsSize)
        {
            l_vertexArray->addValuesToAttribPointer(2, l_modelBuilder->getModel()->m_textureCoords);
        }

        if(l_normalCoordsSize)
        {
            l_vertexArray->addValuesToAttribPointer(3, l_modelBuilder->getModel()->m_normalCoords);
        }

        if (l_modelBuilder->getModel()->m_indicies.size())
        {
            l_vertexArray->addIndicies(l_modelBuilder->getModel()->m_indicies);
        }
        l_vertexArray->unbindVao();
    }

    return l_vertexArray;
}

}//Meshes
}//McEngine
