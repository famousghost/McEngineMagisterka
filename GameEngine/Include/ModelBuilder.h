#pragma once
#include "Mesh.h"
#include <vector>
#include <memory>
#include <iostream>

namespace McEngine
{
namespace Meshes
{
class ModelBuilder
{
public:
    virtual ~ModelBuilder() {}
    virtual void buildVertexCoordinatesArray() = 0;
    virtual void buildColorValuesArray() = 0;
    virtual void buildTextureCoordinatesArray() = 0;
    virtual void buildNormalValuesArray() = 0;
    virtual void buildIndicies() = 0;
    virtual void reset() = 0;

    virtual std::shared_ptr<Mesh> getMesh() const = 0;
};

}//Meshes
}//GameEngine