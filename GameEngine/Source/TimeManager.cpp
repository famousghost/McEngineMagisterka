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

double TimeManager::getDeltaTime()
{
    return m_deltaTime;
}

void TimeManager::setDeltaTime(double p_deltaTime)
{
    m_deltaTime = p_deltaTime;
}

}//Time
}//McEngine