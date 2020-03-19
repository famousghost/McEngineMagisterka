#include "CubeBuilder.h"
#include "Logger.h"

namespace McEngine
{
namespace Meshes
{

CubeBuilder::CubeBuilder(std::shared_ptr<ModelBuilder> p_modelBuilder)
    :m_modelBuilder(p_modelBuilder)
{
    m_object.m_vertexArray->createVao();
    m_object.m_vertexArray->createVbo();
}

void CubeBuilder::reset()
{
    m_modelBuilder->reset();
    m_object.reset();
    m_object.m_vertexArray->createVao();
    m_object.m_vertexArray->createVbo();
}

ObjectBuilder& CubeBuilder::createModel()
{
    prepareAndAttachVertexArray();
    m_object.m_model = m_modelBuilder->getModel();
    return *this;
}

ObjectBuilder& CubeBuilder::setPosition(glm::vec3 p_vec)
{
    return *this;
}

Object CubeBuilder::getObject() const
{
    return m_object;
}

void CubeBuilder::prepareAndAttachVertexArray()
{
    m_modelBuilder->buildVertexCoordinatesArray();
    auto l_vertexCoordSize = m_modelBuilder->getModel()->m_vertexCoords.size();
    if (l_vertexCoordSize != 0)
    {
        LOG("Adding vertex attrib", McEngine::LogType::INF);
        m_object.m_vertexArray->bindVao();
        m_object.m_vertexArray->addValuesToAttribPointer(0, m_modelBuilder->getModel()->m_vertexCoords);
        m_object.m_vertexArray->unbindVao();
        return;
    }
    std::string l_errorMessage = "Cannot add vertex attrib VertexCoordsSize = " + std::to_string(l_vertexCoordSize);
    LOG(l_errorMessage, McEngine::LogType::WARN);
}

ObjectBuilder& CubeBuilder::prepareAndAttachColorArray(const std::vector<ColorValues>& p_colorValues)
{
    m_modelBuilder->buildColorValuesArray(p_colorValues);
    auto l_colorValueSize = m_modelBuilder->getModel()->m_colorValues.size();
    if(l_colorValueSize != 0)
    {
        LOG("Adding color attrib", McEngine::LogType::INF);
        m_object.m_vertexArray->bindVao();
        m_object.m_vertexArray->addValuesToAttribPointer(1, m_modelBuilder->getModel()->m_colorValues);
        m_object.m_vertexArray->unbindVao();
        return *this;
    }
    std::string l_errorMessage = "Cannot add vertex attrib ColorValueSize = " + std::to_string(l_colorValueSize);
    LOG(l_errorMessage, McEngine::LogType::WARN);
    return *this;
}

ObjectBuilder& CubeBuilder::prepareAndAttachTextureArray(const std::vector<TextureCoords>& p_textureCoords)
{
    m_modelBuilder->buildTextureCoordinatesArray(p_textureCoords);
    auto l_textureCoordsSize = m_modelBuilder->getModel()->m_textureCoords.size();
    if(l_textureCoordsSize != 0)
    {
        LOG("Adding texture attrib", McEngine::LogType::INF);
        m_object.m_vertexArray->bindVao();
        m_object.m_vertexArray->addValuesToAttribPointer(2, m_modelBuilder->getModel()->m_textureCoords);
        m_object.m_vertexArray->unbindVao();
        return *this;
    }
    std::string l_errorMessage = "Cannot add vertex attrib TextureCoordsSize = " + std::to_string(l_textureCoordsSize);
    LOG(l_errorMessage, McEngine::LogType::WARN);
    return *this;
}

ObjectBuilder& CubeBuilder::preapreAndAttachNormalArray()
{
    m_modelBuilder->buildNormalValuesArray();
    auto l_normalCoordsSize = m_modelBuilder->getModel()->m_normalCoords.size();
    if (l_normalCoordsSize != 0)
    {
        LOG("Adding normals attrib", McEngine::LogType::INF);
        m_object.m_vertexArray->bindVao();
        m_object.m_vertexArray->addValuesToAttribPointer(3, m_modelBuilder->getModel()->m_normalCoords);
        m_object.m_vertexArray->unbindVao();
        return *this;
    }
    std::string l_errorMessage = "Cannot add vertex attrib NormalCoordsSize = " + std::to_string(l_normalCoordsSize);
    LOG(l_errorMessage, McEngine::LogType::WARN);
    return *this;
}

}//Meshes
}//GameEngine