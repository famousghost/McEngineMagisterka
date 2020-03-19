#pragma once
namespace McEngine
{
namespace Physics
{
class PhysicsManager
{
public:
    PhysicsManager() = default;
    ~PhysicsManager() = default;

    void start();

    void shutdown();
};
}//Physics
}//McEngine

