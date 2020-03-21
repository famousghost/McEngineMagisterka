#include "GuiManager.h"
#include "ScenesManager.h"
#include <algorithm>
#include "Logger.h"

namespace McEngine
{
namespace Gui
{

void GuiManager::start()
{
    m_currentObject = "";
    m_objectElementSize = 0;
    i = 0;
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
    auto& scenes = Scenes::ScenesManager::getInstace().getScenes();
    ImGui_ImplGlfw_InitForOpenGL(scenes.at(0)->getWindow().getGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init(l_glsl_version);
}

void GuiManager::meshGui(ImVec4& p_clearColor)
{
    auto& l_objects = Scenes::ScenesManager::getInstace().getScenes().at(0)->getObjectManager().getObjects();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static int counter = 1;

    ImGui::Begin("McEngine");
    ImGui::Text("The best engine on whole world");

    static std::vector<std::string> items;
    if (m_objectElementSize < l_objects.size())
    {
        LOG("Object size is bigger than previous", LogType::INF);
        for (std::size_t i = m_objectElementSize; i < l_objects.size(); i++)
        {
            items.push_back(l_objects.at(i).second);
        }

        m_objectElementSize = l_objects.size();
    }
    else if (m_objectElementSize > l_objects.size())
    {
        LOG("Object size is smaller than previous", LogType::INF);
        auto comboItemToDelete = std::find_if(items.begin(), items.end(),
            [&](auto& label)
        {
            return m_currentObject == label;
        });

        items.erase(comboItemToDelete);
        m_currentObject = "";
        m_objectElementSize = l_objects.size();
    }

    if (ImGui::BeginCombo("##combo", m_currentObject.c_str()))
    {
        for (int i = 0; i < items.size(); i++)
        {
            bool is_selected = (m_currentObject == items.at(i));
            if (ImGui::Selectable(items.at(i).c_str(), is_selected))
            {
                m_currentObject = items.at(i);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

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

    ImGui::ColorEdit3("clear color", (float*)&p_clearColor);

    if (ImGui::Button("Delete"))
    {
        auto objIt = std::find_if(l_objects.begin(), l_objects.end(),
            [&](auto& label)
        {
            return m_currentObject == label.second;
        });
        if(objIt != l_objects.end())
        {
            l_objects.erase(objIt);
        }
    }
    if (ImGui::Button("Add"))
    {
        std::string label = "Obj" + std::to_string(++i);
        auto& l_objManager = Scenes::ScenesManager::getInstace().getScenes().at(0)->getObjectManager();
        l_objManager.addDefaultObject(label, Meshes::DefaultObjectType::CUBE);
    }

    ImGui::SameLine();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Text("@copyright Marcin Czkeaj");

    ImGui::End();
    ImGui::Render();
}

}//Gui
}//McEngine
