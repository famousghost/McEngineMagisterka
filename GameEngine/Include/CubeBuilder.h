#pragma once
#include <memory>
#include "Object.h"
#include "ObjectBuilder.h"
#include "ModelBuilder.h"

namespace McEngine
{
namespace Meshes
{
class CubeBuilder : public ObjectBuilder
{
public:
    CubeBuilder(std::shared_ptr<ModelBuilder> p_modelBuilder);
    void reset() override;
    ObjectBuilder& createModel() override;
    ObjectBuilder& prepareAndAttachColorArray(const std::vector<ColorValues>& p_colorValues) override;
    ObjectBuilder& prepareAndAttachTextureArray(const std::vector<TextureCoords>& p_textureCoords) override;
    ObjectBuilder& preapreAndAttachNormalArray() override;
    ObjectBuilder& setPosition(glm::vec3 p_vec) override;
    Object getObject() const override;
private:
    void prepareAndAttachVertexArray();
    Object m_object;
    std::shared_ptr<ModelBuilder> m_modelBuilder;
};

}//Meshes
}//GameEngine