#pragma once
namespace McEngine
{
namespace Time
{

class TimeManager
{
public:

    void start();
    void shutdown();

    static TimeManager& getInstance();

    double getDeltaTime();

    void setDeltaTime(double p_deltaTime);

private:
    TimeManager() = default;
    double m_deltaTime;
};
}//Time
}//McEngine

