#pragma once
#include "ScenesManager.h"
namespace McEngine
{
namespace Inputs
{
class InputManager
{
public:
    static float s_lastX;
    static float s_lastY;
    static float s_xOffset;
    static float s_yOffset;


    static bool s_canMoveCamera;
    static bool s_canRotateCamera;

    static GLfloat s_cameraMoveSpeedOnAxisZ;
    static GLfloat s_cameraMoveSpeedOnAxisX;
    static GLfloat s_cameraMoveSpeedOnAxisY;
    static GLfloat s_cameraRotateSpeedOnAxisX;
    static GLfloat s_cameraRotateSpeedOnAxisY;
    static GLfloat s_cameraRotateSpeedOnAxisZ;

    static GLfloat s_changeStateOfMixTextures;
    static GLfloat s_xAxis;
    static GLfloat s_yAxis;

    static double s_fov;

    static InputManager& getInstance();

    void start();

    void shutdown();

private:
    static void keyCallBack(GLFWwindow * p_window, int p_key, int p_scancode, int p_state, int p_mods);

    static void mouseCallBack(GLFWwindow * p_window, double p_xPos, double p_yPos);

    static void scrollCallBack(GLFWwindow * p_window, double p_xOffset, double p_yOffset);

    InputManager() = default;
    ~InputManager() = default;
};

}//Inputs
}//McEngine

