#include "ShaderLoader.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace McEngine
{
namespace Shaders
{

std::string ShaderLoader::loadShaderFromFile(std::string p_shaderPath)
{
    std::ifstream l_shader;
    std::stringstream l_shaderSourceStream;
    std::string l_shaderSource;
    l_shader.open(p_shaderPath);

    if(not l_shader.is_open())
    {   
        LOG("Cannot find shaderfile: '" + p_shaderPath + "'", LogType::WARN);
        return "";
    }

    l_shaderSourceStream << l_shader.rdbuf();
    l_shaderSource = l_shaderSourceStream.str();
    l_shader.close();

    return l_shaderSource;
}

}//Shaders
}//GameEngine
