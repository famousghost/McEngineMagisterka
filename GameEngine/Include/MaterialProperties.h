#pragma once
namespace McEngine
{
namespace Meshes
{
enum class MaterialType
{
    ROCK,      //Density : 0.6  Restitution : 0.1
    WOOD,      //Density : 0.3  Restitution : 0.2
    METAL,      //Density : 1.2  Restitution : 0.05
    BOUNCY_BALL, //Density : 0.3  Restitution : 0.8
    SUPER_BALL,  //Density : 0.3  Restitution : 0.95
    PILLOW,    //Density : 0.1  Restitution : 0.2
    STATIC,    //Density : 0.0  Restitution : 0.4
};

struct MaterialProperties
{
    MaterialType m_materialType;
    float m_density;
    float m_restitution;

    MaterialProperties()
    {
        m_density = 0.0f;
        m_restitution = 0.4f;
    }
};
}//Meshes
}//McEngine

