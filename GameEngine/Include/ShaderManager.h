#pragma once
#include "Shader.h"
#include <map>
namespace McEngine
{
namespace Shaders
{
class ShaderManager
{
public:
    static ShaderManager& getInstance();

    void start();

    void shutdown();

    void addShader(std::string p_shaderLabel, std::string p_vertexShader, std::string p_fragmentShader);
    std::shared_ptr<Shader>& getShader(const std::string& p_label);
private:    
    std::map<std::string, std::shared_ptr<Shader>> m_shaders;

    ShaderManager() = default;
    ~ShaderManager() = default;

};

}//Shaders
}//GameEngine