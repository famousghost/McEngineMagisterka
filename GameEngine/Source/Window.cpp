#include "Window.h"
#include "Logger.h"
#include <iostream>
#include <vector>
#include <algorithm>

namespace McEngine
{
namespace GameWindow
{
Window::Window(const char* p_title, int p_width, int p_height)
    :m_width(p_width), m_height(p_height)
{
    createWindow(p_title, m_width, m_height);
}

Window::~Window()
{
    glfwTerminate();
}

void Window::createWindow(const char* p_title, int p_width, int p_height)
{
    m_window = glfwCreateWindow(p_width, p_height, p_title, nullptr, nullptr);

    if (not m_window)
    {
        LOG("Cannot create window", LogType::ERR);
        exit(0);
    }
}

void Window::setCurrentContext()
{
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallBack);
}

void Window::frameBufferSizeCallBack(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::poolEvents()
{
    glfwPollEvents();
}

void Window::swapBuffer()
{
    glfwSwapBuffers(m_window);
}

GLFWwindow * Window::getGlfwWindow() const
{
    return m_window;
}

bool Window::isOpen()
{
    return not glfwWindowShouldClose(m_window);
}

}//GameWindow
}//GameEngine
