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
    virtual ModelBuilder& buildVerticies() = 0;

    virtual std::shared_ptr<Mesh> getMesh() const = 0;
};

}//Meshes
}//GameEngine