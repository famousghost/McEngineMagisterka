#pragma once
#include <string>

namespace McEngine
{
namespace Shaders
{

class ShaderLoader
{
    public:
        std::string loadShaderFromFile(std::string p_shaderPath);    
};

}//Shaders
}//GameEngine