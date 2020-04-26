#pragma once
#include <string>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "Object.h"
#include <vector>
#include <utility>

namespace McEngine
{
namespace GameWindow
{

class Window
{
public:
    Window(const char * p_title, int p_width, int p_height);
    Window() = default;
    ~Window();

    void createWindow(const char* p_title, int p_width, int p_height);

    void setCurrentContext();

    static void frameBufferSizeCallBack(GLFWwindow * window, int width, int height);

    void poolEvents();

    void swapBuffer();

    GLFWwindow* getGlfwWindow() const;

    bool isOpen();

private:
    GLFWwindow* m_window;
    GLuint m_width;
    GLuint m_height;
};

} // GameWindow
} // GameEngine

