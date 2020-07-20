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

    void chooseObjectViaMouse(std::string p_objectLabel);

    bool getColliderVisiblity() const;

    std::string getCurrentAviableObject() const;

    std::string getMaterialType() const;

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
    std::string m_currentCollider;
    std::string m_currentObjectToAdd;
    std::string m_currentShader;
    std::string m_currentTexture;
    std::string m_newObjectPath;
    std::string m_logsInformation;
    std::string m_colliderTypeName;
    std::string m_materialType;
    Meshes::ColliderType m_colliderType;
    std::size_t m_elementNumber;
    bool m_colidersVisiblity;

    void addObject(std::vector<std::string>& p_items, std::vector<std::string>& p_colliders);
    void colliderMoveOperations(Meshes::Object & p_object);
    void objectMoveOperations();
    void setObjectProperties();
    void deleteObject(std::vector<std::string>& p_items, std::vector<std::string>& p_colliders);
    void updateShaderComboBox(std::vector<std::string>& p_shaderItems);
    void choosingObjectToAddComboBox(std::vector<std::string>& p_shaderItems);
    void updateTextureComboBox(std::vector<std::string>& p_textureItems);
    void objectChoosingComboBox(std::vector<std::string>& p_items, std::vector<std::string>& p_colliders);
    void colliderChoosingComboBox(std::vector<std::string>& p_items);
    void colliderTypeChoosingComboBox(std::vector<std::string>& p_items);
    void choosingObjectMaterialType(std::vector<std::string>& p_materialTypes);
    void updateObjectMassProperties();
    void updateListOfObjects(std::vector<std::string>& p_shaderItems);
    void updateListOfShaders(std::vector<std::string>& p_shadersToAdd);
    void updateObjectShader();
    void refreshShader();
};
}//Gui
}//McEngine

