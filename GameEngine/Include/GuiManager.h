#pragma once
#include <vector>
#include <string>
#include "Object.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace McEngine
{
namespace Gui
{
class GuiManager
{
public:
    void start();
    
    void shutdown();

    void meshGui(ImVec4 & p_clearColor);

    static GuiManager& getInstance()
    {
        static GuiManager guiManager;
        return guiManager;
    }

private:
    GuiManager() = default;
    ~GuiManager() = default;
    void initImGui();
    std::size_t m_objectElementSize;
    std::string m_currentObject;
    int i;
};
}//Gui
}//McEngine

