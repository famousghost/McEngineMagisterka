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

    double getDeltaTime() const;

    double getAccumulator() const;

    void setDeltaTime(double p_deltaTime);

    void setAccumulator(double p_deltaTime);

private:
    TimeManager() = default;
    double m_deltaTime;
    double m_accumulator;
};
}//Time
}//McEngine

