#include "TimeManager.h"

namespace McEngine
{
namespace Time 
{

void TimeManager::start()
{
    m_deltaTime = 0.0;
}

void TimeManager::shutdown()
{
}
TimeManager & TimeManager::getInstance()
{
    static TimeManager timeManager;
    return timeManager;
}

double TimeManager::getDeltaTime() const
{
    return m_deltaTime;
}

double TimeManager::getAccumulator() const
{
    return m_accumulator;
}

float TimeManager::getCurrentTime() const
{
    return m_currentTime;
}

void TimeManager::setDeltaTime(double p_deltaTime)
{
    m_deltaTime = p_deltaTime;
}

void TimeManager::setCurrentTime(float p_currentTime)
{
    m_currentTime = p_currentTime;
}

void TimeManager::setAccumulator(double p_deltaTime)
{
    m_accumulator += p_deltaTime;
    if (m_accumulator > 0.2f)
        m_accumulator = 0.2f;
}

}//Time
}//McEngine