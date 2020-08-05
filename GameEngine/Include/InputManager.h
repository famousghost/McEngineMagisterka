#pragma once
#include "ScenesManager.h"
#include "WindowManager.h"
#include <map>

namespace McEngine
{
namespace Inputs
{
class InputManager
{
public:
    static std::map<int, bool> s_keyValues;

    static float s_lastX;
    static float s_lastY;
    static float s_xOffset;
    static float s_yOffset;

    static bool s_canMoveCamera;
    static bool s_canRotateCamera;
    static bool s_onClickMouse;

    static int s_cameraFrontKey;
    static int s_cameraBackKey;
    static int s_cameraLeftKey;
    static int s_cameraRightKey;

    static GLfloat s_cameraMoveSpeedOnAxisZ;
    static GLfloat s_cameraMoveSpeedOnAxisX;
    static GLfloat s_cameraMoveSpeedOnAxisY;
    static GLfloat s_cameraRotateSpeedOnAxisX;
    static GLfloat s_cameraRotateSpeedOnAxisY;
    static GLfloat s_cameraRotateSpeedOnAxisZ;
    static GLfloat s_gameCameraMoveSpeedOnAxisZ;
    static GLfloat s_gameCameraMoveSpeedOnAxisX;
    static GLfloat s_gameCameraMoveSpeedOnAxisY;
    static GLfloat s_gameCameraRotateSpeedOnAxisX;
    static GLfloat s_gameCameraRotateSpeedOnAxisY;
    static GLfloat s_gameCameraRotateSpeedOnAxisZ;

    static InputManager& getInstance();

    static bool getKeyDown(int p_keyValue);

    void setKeyUp(int p_keyValue);

    void start();

    void shutdown();

private:
    static void keyCallBack(GLFWwindow * p_window, int p_key, int p_scancode, int p_state, int p_mods);

    static void keyDown(int p_key);

    static void keyUp(int p_key);

    static void editorCameraPress(GLFWwindow * p_window, int p_key);

    static void editorCameraRelease(GLFWwindow * p_window, int p_key);

    static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);

    static void mouseCallBack(GLFWwindow * p_window, double p_xPos, double p_yPos);

    static void scrollCallBack(GLFWwindow * p_window, double p_xOffset, double p_yOffset);

    InputManager() = default;
    ~InputManager() = default;
};

}//Inputs
}//McEngine

