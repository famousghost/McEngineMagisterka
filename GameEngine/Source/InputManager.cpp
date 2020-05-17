#include "InputManager.h"

namespace McEngine
{
namespace Inputs
{

float InputManager::s_lastX = 0.0f;
float InputManager::s_lastY = 0.0f;
float InputManager::s_xOffset = 0.0f;
float InputManager::s_yOffset = 0.0f;


bool InputManager::s_canMoveCamera = false;
bool InputManager::s_canRotateCamera = false;
bool InputManager::s_onClickMouse = false;

GLfloat InputManager::s_cameraMoveSpeedOnAxisZ = 0.0f;
GLfloat InputManager::s_cameraMoveSpeedOnAxisX = 0.0f;
GLfloat InputManager::s_cameraMoveSpeedOnAxisY = 0.0f;
GLfloat InputManager::s_cameraRotateSpeedOnAxisX = 0.0f;
GLfloat InputManager::s_cameraRotateSpeedOnAxisY = 0.0f;
GLfloat InputManager::s_cameraRotateSpeedOnAxisZ = 0.0f;
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
    auto* l_window = GameWindow::WindowManager::getInstance().getWindow().getGlfwWindow();
    if (p_state == GLFW_PRESS)
    {
        if (p_key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(p_window, true);
        }
        if (p_key == GLFW_KEY_LEFT_CONTROL)
        {
            s_canMoveCamera = true;
        }
        if (p_key == GLFW_KEY_LEFT_ALT)
        {
            glfwSetInputMode(l_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            s_canRotateCamera = true;
        }
    }
    else if (p_state == GLFW_RELEASE)
    {
        if (p_key == GLFW_KEY_LEFT_CONTROL)
        {
            s_canMoveCamera = false;
        }
        if (p_key == GLFW_KEY_LEFT_ALT)
        {
            glfwSetInputMode(l_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            s_canRotateCamera = false;
        }
    }
}

void InputManager::mouse_button_callback(GLFWwindow* p_window, int p_button, int p_action, int p_mods)
{
    auto* l_window = GameWindow::WindowManager::getInstance().getWindow().getGlfwWindow();
    if (p_action == GLFW_PRESS)
    {
        if (p_button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            glfwSetInputMode(l_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            s_canRotateCamera = true;
        }
        if (p_button == GLFW_MOUSE_BUTTON_LEFT)
        {
            s_onClickMouse = true;
        }
    }
    if (p_action == GLFW_RELEASE)
    {
        if (p_button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            glfwSetInputMode(l_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            s_canRotateCamera = false;
        }
        if (p_button == GLFW_MOUSE_BUTTON_LEFT)
        {
            s_onClickMouse = false;
        }
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

    if (s_canMoveCamera)
    {
        s_cameraMoveSpeedOnAxisX = s_xOffset;
        s_cameraMoveSpeedOnAxisY = s_yOffset;
    }
    if (s_canRotateCamera)
    {
        s_cameraRotateSpeedOnAxisX = s_xOffset;
        s_cameraRotateSpeedOnAxisY = s_yOffset;
    }
}

void InputManager::scrollCallBack(GLFWwindow * p_window, double p_xOffset, double p_yOffset)
{
    if(s_canMoveCamera)
    {
        s_cameraMoveSpeedOnAxisZ = p_yOffset;
    }
}

void InputManager::start()
{
    auto* l_currentWindow = GameWindow::WindowManager::getInstance().getWindow().getGlfwWindow();
    glfwSetInputMode(l_currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetKeyCallback(l_currentWindow, keyCallBack);
    glfwSetCursorPosCallback(l_currentWindow, mouseCallBack);
    glfwSetScrollCallback(l_currentWindow, scrollCallBack);
    glfwSetMouseButtonCallback(l_currentWindow, mouse_button_callback);
}

void InputManager::shutdown()
{
    
}

}//Inputs
}//McEngine