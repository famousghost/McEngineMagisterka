#include "InputManager.h"


namespace McEngine
{
namespace Inputs
{

float InputManager::s_lastX = 400.0f;
float InputManager::s_lastY = 300.0f;
float InputManager::s_xOffset = 0.0f;
float InputManager::s_yOffset = 0.0;

//move our camera in 3d space
bool InputManager::s_moveFront = false;
bool InputManager::s_moveRight = false;
bool InputManager::s_moveBack = false;
bool InputManager::s_moveLeft = false;


GLfloat InputManager::s_changeStateOfMixTextures = 0.0f;
GLfloat InputManager::s_xAxis = 0.0f;
GLfloat InputManager::s_yAxis = 0.0f;

double InputManager::s_fov = 45.0f;

InputManager & InputManager::getInstance()
{
    static InputManager inputManager;
    return inputManager;
}

void InputManager::keyCallBack(GLFWwindow * p_window, int p_key, int p_scancode, int p_state, int p_mods)
{
    if (p_state == GLFW_PRESS)
    {
        if (p_key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(p_window, true);
        }
    }
    else if (p_state == GLFW_RELEASE)
    {

    }
}

void InputManager::mouseCallBack(GLFWwindow * p_window, double p_xPos, double p_yPos)
{
    static bool l_firstTimeSetCamera = true;
    if (l_firstTimeSetCamera)
    {
        s_lastX = p_xPos;
        s_lastY = p_yPos;
        l_firstTimeSetCamera = false;
    }

    s_xOffset = p_xPos - s_lastX;
    s_yOffset = s_lastY - p_yPos;

    s_lastX = p_xPos;
    s_lastY = p_yPos;
}

void InputManager::scrollCallBack(GLFWwindow * p_window, double p_xOffset, double p_yOffset)
{
    if (s_fov >= 1.0f && s_fov <= 45.0f)
        s_fov -= p_yOffset;
    if (s_fov <= 1.0f)
        s_fov = 1.0f;
    if (s_fov >= 45.0f)
        s_fov = 45.0f;
}

void InputManager::start()
{
    auto& l_scene = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene();
    glfwSetInputMode(l_scene->getWindow().getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(l_scene->getWindow().getGlfwWindow(), keyCallBack);
    glfwSetCursorPosCallback(l_scene->getWindow().getGlfwWindow(), mouseCallBack);
    glfwSetScrollCallback(l_scene->getWindow().getGlfwWindow(), scrollCallBack);
}

void InputManager::shutdown()
{
    
}

}//Inputs
}//McEngine