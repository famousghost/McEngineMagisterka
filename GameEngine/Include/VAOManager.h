#pragma once
#include "VertexArray.h"
#include "CubeModelBuilder.h"
#include <map>
#include <string>

namespace McEngine
{
namespace Meshes
{
class VAOManager
{
public:
    static VAOManager& getInstance();

    void start();

    void shutdown();

    void addDefaultVertexArrayObjects();

    std::shared_ptr<VertexArray> getVertexArray(std::string p_label) const;

private:
    std::map<std::string, std::shared_ptr<VertexArray>> m_vertexArray;

    std::shared_ptr<VertexArray> createDefaultVertexArrayObject(std::string p_label);

    VAOManager() = default;
    ~VAOManager() = default;
};

}//Meshes
}//McEngine