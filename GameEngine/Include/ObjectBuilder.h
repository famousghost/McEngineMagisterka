#pragma once
#include "Object.h"
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

namespace McEngine
{
namespace Meshes
{

class ObjectBuilder
{
    public:
        virtual ObjectBuilder& createModel() = 0;
        virtual ObjectBuilder& setPosition(glm::vec3) = 0;
        virtual ObjectBuilder& prepareAndAttachColorArray(const std::vector<ColorValues>&) = 0;
        virtual ObjectBuilder& prepareAndAttachTextureArray(const std::vector<TextureCoords>&) = 0;
        virtual ObjectBuilder& preapreAndAttachNormalArray() = 0;
        virtual void reset() = 0;

        virtual Object getObject() const = 0;
};

}//Meshes
}//GameEngine