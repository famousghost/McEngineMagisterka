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

    void meshGui();

    void updateObjectTetxture();

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
    std::string m_currentObjectToAdd;
    std::string m_currentShader;
    std::string m_currentTexture;
    std::string m_newObjectPath;
    std::size_t m_elementNumber;

    void addObject(std::vector<std::string>& p_items);
    void objectMoveOperations();
    void setObjectProperties();
    void deleteObject(std::vector<std::string>& p_items);
    void updateShaderComboBox(std::vector<std::string>& p_shaderItems);
    void choosingObjectToAddComboBox(std::vector<std::string>& p_shaderItems);
    void updateTextureComboBox(std::vector<std::string>& p_textureItems);
    void objectChoosingComboBox(std::vector<std::string>& p_items);
    void updateListOfObjects(std::vector<std::string>& p_shaderItems);
    void updateObjectShader();
};
}//Gui
}//McEngine

