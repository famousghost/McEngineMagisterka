#include "GuiManager.h"
#include "ScenesManager.h"
#include "PrefabManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "WindowManager.h"
#include "RenderManager.h"
#include "FilePathParser.h"
#include <algorithm>
#include "Logger.h"

namespace McEngine
{
namespace Gui
{

void GuiManager::start()
{
    m_currentShader = "defaultShader";
    m_currentObject = "";
    m_currentObjectToAdd = "Cube";
    m_currentTexture = "Wall";
    m_objectElementSize = 0;
    m_elementNumber = 0;
    m_colidersVisiblity = false;
    initImGui();
}

void GuiManager::shutdown()
{
    
}

void GuiManager::initImGui()
{
    const char* l_glsl_version = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    auto& scene = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene();
    auto* l_window = GameWindow::WindowManager::getInstance().getWindow().getGlfwWindow();
    ImGui_ImplGlfw_InitForOpenGL(l_window, true);
    ImGui_ImplOpenGL3_Init(l_glsl_version);
}

void GuiManager::meshGui()
{
    auto& l_objectManager = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager();
    auto& l_objects = l_objectManager.getObjects();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("McEngine");
    ImGui::Text("Simple game engine");

    static std::vector<std::string> items;
    static std::vector<std::string> shadersItems = {"defaultShader", 
                                                    "colorShader", 
                                                    "diffuseShader", 
                                                    "textureShader", 
                                                    "customObjectShader"};

    static std::vector<std::string> textureItems = {"Wall", 
                                                    "Awsomeface", 
                                                    "Face_Side1"};

    static std::vector<std::string> objectsToAdd = { "Cube", 
                                                     "Sphere", 
                                                     "Cylinder", 
                                                     "Cone", 
                                                     "Torus", 
                                                     "Monkeyhead", 
                                                     "Nanosuit", 
                                                     "Plane"};
    objectChoosingComboBox(items);
    choosingObjectToAddComboBox(objectsToAdd);
    updateShaderComboBox(shadersItems);
    updateTextureComboBox(textureItems);
    objectMoveOperations();
    setObjectProperties();

    auto l_backgroundColor = GameWindow::WindowManager::getInstance().getBackgroundColor();
    ImGui::ColorEdit3("Change Color", (float*)&l_backgroundColor);

    addObject(items);
    deleteObject(items);
    updateObjectShader();
    updateObjectTetxture();
    updateListOfObjects(objectsToAdd);

    static std::string buttonName = "show collider";
    if (ImGui::Button(buttonName.c_str()))
    {
        if(buttonName == "show collider")
        {
            m_colidersVisiblity = true;
            buttonName = "hide collider";
        }
        else
        {
            buttonName = "show collider";
            m_colidersVisiblity = false;
        }
    }

    auto& l_renderManager = Renderer::RenderManager::getInstance();

    if (ImGui::Button("Show Raw Mesh"))
    {
        l_renderManager.showMesh();
    }
    if (ImGui::Button("Fill Mesh"))
    {
        l_renderManager.fillMesh();
    }

    if (ImGui::Button("Reload Shaders"))
    {
        Shaders::ShaderManager::getInstance().resetShaders();
    }

    
    ImGui::SliderFloat3("LightPosition", &l_objectManager.m_lightPosition.x, -10.0f, 10.0f);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("@copyright Marcin Czekaj");

    ImGui::End();
    ImGui::Render();
}

void GuiManager::updateListOfObjects(std::vector<std::string>& p_objectsToAdd)
{
    static char l_filePath[1000];
    ImGui::InputText("##Chatbox", l_filePath, 20, ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::Button("Add Object To List"))
    {
        auto l_objectName = Utility::FilePathParser::fetchObjectName(l_filePath);
        auto& l_prefabManager = Meshes::PrefabManager::getInstance();
        if (l_prefabManager.getMesh(l_objectName))
        {
            return;
        }
        l_prefabManager.loadMeshFromFile(l_filePath);
        p_objectsToAdd.push_back(l_objectName);
        strcpy(l_filePath, "");
    }
}

void GuiManager::updateObjectTetxture()
{
    auto& l_objects =
        Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager().getObjects();
    if (ImGui::Button("Update texture"))
    {
        auto objIt = std::find_if(l_objects.begin(), l_objects.end(),
            [&](auto& label)
        {
            return m_currentObject == label.second;
        });
        if (objIt != l_objects.end())
        {
            try
            {
                objIt->first.m_currentAvaiableTexture = m_currentTexture;
            }
            catch (std::exception& ex)
            {
                LOG("Cannot find texture, there is no texture with this name", LogType::WARN);
            }
        }
    }
}

void GuiManager::chooseObjectViaMouse(std::string p_objectLabel)
{
    m_currentObject = p_objectLabel;
}

bool GuiManager::getColliderVisiblity() const
{
    return m_colidersVisiblity;
}

void GuiManager::addObject(std::vector<std::string>& p_items)
{
    auto& l_objects =
        Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager().getObjects();
    if (m_objectElementSize < l_objects.size())
    {
        LOG("Object size is bigger than previous", LogType::INF);
        for (std::size_t i = m_objectElementSize; i < l_objects.size(); i++)
        {
            p_items.push_back(l_objects.at(i).second);
        }

        m_objectElementSize = l_objects.size();
    }

    if (ImGui::Button("Add Object"))
    {
        std::string label = "Obj" + std::to_string(++m_elementNumber);
        auto& l_objManager = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager();
        l_objManager.addCustomObject(label, m_currentObjectToAdd, m_currentShader);
        m_currentObject = label;
    }
}

void GuiManager::objectMoveOperations()
{
    auto& l_objects =
        Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager().getObjects();
    auto objIt = std::find_if(l_objects.begin(), l_objects.end(),
        [&](auto& label)
    {
        return m_currentObject == label.second;
    });

    if (objIt != l_objects.end())
    {
        auto& l_transform = objIt->first.m_transform;
        ImGui::SliderFloat3("Translation", &l_transform.m_position.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotatione", &l_transform.m_rotatione.x, -360.0f, 360.0f);
        ImGui::SliderFloat3("Scale", &l_transform.m_scale.x, -10.0f, 10.0f);
    }
}

void GuiManager::setObjectProperties()
{
    auto& l_objects =
        Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager().getObjects();
    auto objIt = std::find_if(l_objects.begin(), l_objects.end(),
        [&](auto& label)
    {
        return m_currentObject == label.second;
    });

    if (objIt != l_objects.end())
    {
        auto& l_material = objIt->first.m_material;
        ImGui::SliderFloat3("AmbientLight", &l_material.m_ambientLight.x, 0.0f, 1.0f);
        ImGui::SliderFloat3("DiffuseLight", &l_material.m_diffuseLight.x, 0.0f, 1.0f);
        ImGui::SliderFloat3("SpecularLight", &l_material.m_specularLight.x, 0.0f, 1.0f);
        ImGui::SliderFloat3("ObjectColor", &l_material.m_objectColor.x, 0.0f, 1.0f);
    }
}

void GuiManager::deleteObject(std::vector<std::string>& p_items)
{
    auto& l_objects = 
        Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager().getObjects();

    if (m_objectElementSize > l_objects.size())
    {
        LOG("Object size is smaller than previous", LogType::INF);
        auto comboItemToDelete = std::find_if(p_items.begin(), p_items.end(),
            [&](auto& label)
        {
            return m_currentObject == label;
        });

        if(comboItemToDelete != p_items.end())
        {
            p_items.erase(comboItemToDelete);
            m_currentObject = "";
            m_objectElementSize = l_objects.size();
        }
    }
    if (ImGui::Button("Delete Object"))
    {
        auto objIt = std::find_if(l_objects.begin(), l_objects.end(),
            [&](auto& label)
        {
            return m_currentObject == label.second;
        });
        if (objIt != l_objects.end())
        {
            l_objects.erase(objIt);
        }
    }
}

void GuiManager::updateShaderComboBox(std::vector<std::string>& p_shaderItems)
{
    if (ImGui::BeginCombo("##shaderCombo", m_currentShader.c_str()))
    {
        for (int i = 0; i < p_shaderItems.size(); i++)
        {
            bool is_selected = (m_currentShader == p_shaderItems.at(i));
            if (ImGui::Selectable(p_shaderItems.at(i).c_str(), is_selected))
            {
                m_currentShader = p_shaderItems.at(i);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void GuiManager::choosingObjectToAddComboBox(std::vector<std::string>& p_shaderItems)
{
    if (ImGui::BeginCombo("##objectToAddCombo", m_currentObjectToAdd.c_str()))
    {
        for (int i = 0; i < p_shaderItems.size(); i++)
        {
            bool is_selected = (m_currentObjectToAdd == p_shaderItems.at(i));
            if (ImGui::Selectable(p_shaderItems.at(i).c_str(), is_selected))
            {
                m_currentObjectToAdd = p_shaderItems.at(i);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void GuiManager::updateTextureComboBox(std::vector<std::string>& p_textureItems)
{
    if (ImGui::BeginCombo("##textureCombo", m_currentTexture.c_str()))
    {
        for (int i = 0; i < p_textureItems.size(); i++)
        {
            bool is_selected = (m_currentTexture == p_textureItems.at(i));
            if (ImGui::Selectable(p_textureItems.at(i).c_str(), is_selected))
            {
                m_currentTexture = p_textureItems.at(i);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void GuiManager::objectChoosingComboBox(std::vector<std::string>& p_items)
{

    if (ImGui::BeginCombo("##objectCombo", m_currentObject.c_str()))
    {
        for (int i = 0; i < p_items.size(); i++)
        {
            bool is_selected = (m_currentObject == p_items.at(i));
            if (ImGui::Selectable(p_items.at(i).c_str(), is_selected))
            {
                m_currentObject = p_items.at(i);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void GuiManager::updateObjectShader()
{
    auto& l_objects = 
        Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager().getObjects();
    if (ImGui::Button("Update Shader"))
    {
        auto objIt = std::find_if(l_objects.begin(), l_objects.end(),
            [&](auto& label)
        {
            return m_currentObject == label.second;
        });
        if (objIt != l_objects.end())
        {
            try
            {
                objIt->first.m_shaderProgram = Shaders::ShaderManager::getInstance().getShader(m_currentShader);
            }
            catch (std::exception& ex)
            {
                LOG("Cannot find shader, there is no shader with this name", LogType::ERR);
            }
        }
    }
}

}//Gui
}//McEngine
