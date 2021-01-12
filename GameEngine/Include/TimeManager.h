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

    float getCurrentTime() const;

    void setDeltaTime(double p_deltaTime);

    void setCurrentTime(float p_currentTime);

    void setAccumulator(double p_deltaTime);

private:
    TimeManager() = default;
    double m_deltaTime;
    float m_currentTime;
    double m_accumulator;
};
}//Time
}//McEngine

