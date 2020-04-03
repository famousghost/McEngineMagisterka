#pragma once
#include "Model.h"
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

    virtual std::shared_ptr<Model> getModel() const = 0;
};

}//Meshes
}//GameEngine