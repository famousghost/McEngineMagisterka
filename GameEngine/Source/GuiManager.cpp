#include "GuiManager.h"
#include "ScenesManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
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
    m_currentTexture = "texture";
    m_objectElementSize = 0;
    m_elementNumber = 0;
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
    ImGui_ImplGlfw_InitForOpenGL(scene->getWindow().getGlfwWindow(), true);
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
    ImGui::Text("The best engine on whole world");

    static std::vector<std::string> items;
    static std::vector<std::string> shadersItems = { "defaultShader", "colorShader", "diffuseShader", "textureShader"};
    static std::vector<std::string> textureItems = {"texture", "texture2", "texture3"};
    objectChoosingComboBox(items);
    updateShaderComboBox(shadersItems);
    updateTextureComboBox(textureItems);
    objectMoveOperations();
    auto& scene =
        Scenes::ScenesManager::getInstace().getCurrentAvaiableScene();
    ImGui::ColorEdit3("Change Color", (float*)&scene->m_backgroundColor);

    addObject(items);
    deleteObject(items);
    updateObjectShader();
    updateObjectTetxture();



    ImGui::SameLine();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("@copyright Marcin Czkeaj");

    ImGui::End();
    ImGui::Render();
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

    if (ImGui::Button("Add Cube"))
    {
        std::string label = "Obj" + std::to_string(++m_elementNumber);
        auto& l_objManager = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager();
        l_objManager.addDefaultObject(label, Meshes::DefaultObjectType::CUBE, m_currentShader);
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
        ImGui::SliderFloat3("Translation", &objIt->first.m_position.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotatione", &objIt->first.m_rotatione.x, -360.0f, 360.0f);
        ImGui::SliderFloat3("Scale", &objIt->first.m_scale.x, -10.0f, 10.0f);
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
