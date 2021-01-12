#include "GuiManager.h"
#include "ScenesManager.h"
#include "PrefabManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "WindowManager.h"
#include "RenderManager.h"
#include "FilePathParser.h"
#include "PhysicsManager.h"
#include "InputManager.h"
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
    m_cuurentNormalTexture = "";
    m_materialType = "ROCK";
    m_currentObjectToAdd = "Cube";
    m_currentTexture = "Wall";
    m_objectElementSize = 0;
    m_elementNumber = 0;
    m_colidersVisiblity = false;
    m_colliderTypeName = "OBB";
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

void GuiManager::renderGui()
{
    auto& l_objectManager = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager();
    auto& l_objects = l_objectManager.getObjects();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("McEngine");
    ImGui::Text("Simple graphics engine");

    static std::vector<std::string> items;

    static std::vector<std::string> shadersItems = {"defaultShader", 
                                                    "diffuseShader", 
                                                    "textureShader", 
                                                    "customObjectShader",
                                                    "terrainShader"};

    static std::vector<std::string> textureItems = {"Wall", 
                                                    "Awsomeface", 
                                                    "Face_Side1"};

    static std::vector<std::string> normalTextureItems;

    static std::vector<std::string> objectsToAdd = { "Cube", 
                                                     "Sphere", 
                                                     "Cylinder", 
                                                     "Cone", 
                                                     "Torus", 
                                                     "Monkeyhead", 
                                                     "Nanosuit", 
                                                     "Plane",
                                                     "Terrain"};

    static std::vector<std::string> colliderTypes = {"OBB",
                                                     "AABB",
                                                     "SPHERE"};
    static std::vector<std::string> colliders;

    static std::vector<std::string> materialTypes = {"ROCK",
                                                     "WOOD",
                                                     "METAL",
                                                     "BOUNCY_BALL",
                                                     "SUPER_BALL",
                                                     "PILLOW",
                                                     "STATIC"};

    ImGui::Text("Objects list:");
    objectChoosingComboBox(items, colliders);
    ImGui::Text("Collider type list:");
    colliderTypeChoosingComboBox(colliderTypes);
    ImGui::Text("Colliders list:");
    colliderChoosingComboBox(colliders);
    ImGui::Text("Object to add list:");
    choosingObjectToAddComboBox(objectsToAdd);
    ImGui::Text("Material type:");
    choosingObjectMaterialType(materialTypes);
    ImGui::Text("Shaders list:");
    updateShaderComboBox(shadersItems);
    updateObjectShader();
    refreshShader();
    ImGui::Text("Textures list:");
    updateTextureComboBox(textureItems);
    ImGui::Text("Normal Textures list:");
    updateNormalTextureComboBox(normalTextureItems);
    ImGui::Text("Input new texture (with extension):");
    updateListOfTextures(textureItems);
    ImGui::Text("Input new normal texture (with extension):");
    updateListOfNormalTextures(normalTextureItems);
    updateObjectTetxture();
    objectMoveOperations();
    setObjectProperties();

    auto l_backgroundColor = GameWindow::WindowManager::getInstance().getBackgroundColor();
    ImGui::ColorEdit3("Change Color", (float*)&l_backgroundColor);

    addObject(items, colliders);
    deleteObject(items, colliders);
    ImGui::Text("Input new object name (with extension):");
    updateListOfObjects(objectsToAdd);
    ImGui::Text("Input new shader name:");
    updateListOfShaders(shadersItems);
    updateObjectMassProperties();
    m_currentObject = l_objectManager.cloneObject(m_currentObject);

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

    static std::string rigidBody = "physics_activate";
    if (ImGui::Button(rigidBody.c_str()))
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
            objIt->first.m_isRigidBody = not objIt->first.m_isRigidBody;
        }
    }

    static std::string gravityForce = "gravityForce";
    if (ImGui::Button(gravityForce.c_str()))
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
            objIt->first.m_rigidBody.m_gravityForce = not objIt->first.m_rigidBody.m_gravityForce;
        }
    }

    applyForce();
    
    ImGui::SliderFloat3("LightPosition", &l_objectManager.m_lightPosition.x, -10.0f, 10.0f);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("@copyright Marcin Czekaj");

    ImGui::End();
    ImGui::Render();
}

void GuiManager::updateListOfObjects(std::vector<std::string>& p_objectsToAdd)
{
    static char l_filePath[1000];
    ImGui::InputText("##ObjectChatbox", l_filePath, 20, ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::Button("Add Object To List"))
    {
        auto l_objectName = Utility::FilePathParser::fetchObjectName(l_filePath);
        auto& l_prefabManager = Meshes::PrefabManager::getInstance();
        if (l_prefabManager.getMesh(l_objectName))
        {
            return;
        }
        std::string l_objectPath = "Objects/";
        if (not l_prefabManager.loadMeshFromFile(l_objectPath + l_filePath))
        {
            return;
        }
        p_objectsToAdd.push_back(l_objectName);
        strcpy(l_filePath, "");
    }
}

void GuiManager::updateListOfTextures(std::vector<std::string>& p_textruresToAdd)
{
    static char l_filePath[1000];
    ImGui::InputText("##TextureChatbox", l_filePath, 20, ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::Button("Add Texture To List"))
    {
        auto l_textureIndex = Utility::FilePathParser::fetchObjectName(l_filePath);
        auto& l_textureManager = Textures::TextureManager::getInstance();
        std::string l_texturePath = "Textures/";
        std::string l_textureName = l_filePath;
        if (not l_textureManager.createTexture(l_texturePath + l_textureName, GL_REPEAT, GL_LINEAR, l_textureIndex))
        {
            return;
        }
        p_textruresToAdd.push_back(l_textureIndex);
        strcpy(l_filePath, "");
    }
}

void GuiManager::updateListOfNormalTextures(std::vector<std::string>& p_textruresToAdd)
{
    static char l_filePath[1000];
    ImGui::InputText("##NormalTextureChatbox", l_filePath, 20, ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::Button("Add NormalTexture To List"))
    {
        auto l_textureIndex = Utility::FilePathParser::fetchObjectName(l_filePath);
        auto& l_textureManager = Textures::TextureManager::getInstance();
        std::string l_texturePath = "Textures/NormalMaps/";
        std::string l_textureName = l_filePath;
        if (not l_textureManager.createNormalTexture(l_texturePath + l_textureName, GL_REPEAT, GL_LINEAR, l_textureIndex))
        {
            return;
        }
        p_textruresToAdd.push_back(l_textureIndex);
        strcpy(l_filePath, "");
    }
}

void GuiManager::updateListOfShaders(std::vector<std::string>& p_shadersToAdd)
{
    static char l_filePath[1000];
    ImGui::InputText("##ShaderChatBox", l_filePath, 20, ImGuiInputTextFlags_EnterReturnsTrue);
    if (ImGui::Button("Add Shader To List"))
    {
        auto l_shaderName = Utility::FilePathParser::fetchObjectName(l_filePath);
        auto& l_shaderManager = Shaders::ShaderManager::getInstance();
        if (l_shaderManager.getShader(l_shaderName))
        {
            return;
        }
        std::string l_shaderPath = "Shaders/";
        std::string l_vlsl = ".vlsl";
        std::string l_flsl = ".flsl";
        if (not l_shaderManager.addShader(l_shaderName,
                                          l_shaderPath + l_shaderName + l_vlsl,
                                          l_shaderPath + l_shaderName + l_flsl))
        {
            return;
        }
        p_shadersToAdd.push_back(l_shaderName);
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
                objIt->first.m_currentAvaiableNormalTexture = m_cuurentNormalTexture;
            }
            catch (std::exception& ex)
            {
                LOG("Cannot find texture, there is no texture with this name", LogType::WARN);
            }
        }
    }
}

void GuiManager::applyForce()
{
    static glm::vec3 l_force;
    ImGui::SliderFloat3("force", &l_force.x, -10.0f, 10.0f);

    if (ImGui::Button("applyForce"))
    {
        if(m_force != nullptr)
        {
            *m_force += l_force;
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

std::string GuiManager::getCurrentAviableObject() const
{
    return m_currentObject;
}

std::string GuiManager::getMaterialType() const
{
    return m_materialType;
}

void GuiManager::addObject(std::vector<std::string>& p_items,
                           std::vector<std::string>& p_colliders)
{
    auto& l_objects =
        Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager().getObjects();
    if (m_objectElementSize < l_objects.size())
    {
        LOG("Object size is bigger than previous", LogType::INF);
        for (std::size_t i = m_objectElementSize; i < l_objects.size(); i++)
        {
            m_currentCollider = "";
            p_colliders.clear();
            p_items.push_back(l_objects.at(i).second);
            if(l_objects.at(i).first.m_colider.size() > 0)
            {
                for(auto& l_collider : l_objects.at(i).first.m_colider)
                {
                    p_colliders.push_back(l_collider.m_colliderName);
                }
            }
        }

        m_objectElementSize = l_objects.size();
    }

    if (ImGui::Button("Add Object"))
    {
        std::string label = "Obj" + std::to_string(++m_elementNumber);
        auto& l_objManager = Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager();
        if (m_currentObjectToAdd == "Terrain")
        {
            l_objManager.addTerrain(label, m_currentShader, m_colliderType);
        }
        else
        {
            l_objManager.addCustomObject(label, m_currentObjectToAdd, m_currentShader, m_colliderType);
        }
        m_currentObject = label;
    }
}

void GuiManager::colliderMoveOperations(Meshes::Object& p_object)
{
    auto& l_colliders = p_object.m_colider;
    if (l_colliders.size() == 0 or m_currentCollider.empty())
    {
        return;
    }

    auto colIt = std::find_if(l_colliders.begin(), l_colliders.end(),
        [&](auto& label)
    {
        return m_currentCollider == label.m_colliderName;
    });

    auto& l_transform = colIt->m_transform;
    ImGui::SliderFloat3("Collider Translation", &l_transform.m_position.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Collider Rotatione", &l_transform.m_rotatione.x, -360.0f, 360.0f);
    auto& l_radius = colIt->m_transform.m_radiusScale;
    if (colIt->m_colliderType == Meshes::ColliderType::SPHERE)
    {
        l_transform.m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        ImGui::SliderFloat("Collider Scale", &l_radius, -10.0f, 10.0f);
        l_transform.m_scale *= l_radius;
    }
    else
    {
        ImGui::SliderFloat3("Collider Scale", &l_transform.m_scale.x, -10.0f, 10.0f);
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
        auto& l_obj = objIt->first;
        auto& l_transform = l_obj.m_transform;
        auto& l_radius = l_obj.m_transform.m_radiusScale;
        ImGui::SliderFloat3("Translation", &l_transform.m_position.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("Rotatione", &l_transform.m_rotatione.x, -360.0f, 360.0f);
        if (objIt->first.m_colider.size() != 0)
        {
            if (objIt->first.m_colider.at(0).m_colliderType == Meshes::ColliderType::SPHERE)
            {
                l_transform.m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
                ImGui::SliderFloat("Scale", &l_radius, -10.0f, 10.0f);
                l_transform.m_scale *= l_radius;
            }
            else
            {
                ImGui::SliderFloat3("Scale", &l_transform.m_scale.x, -100.0f, 100.0f);
            }
        }
        else
        {
            ImGui::SliderFloat3("Scale", &l_transform.m_scale.x, -100.0f, 100.0f);
        }
        colliderMoveOperations(l_obj);
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

void GuiManager::deleteObject(std::vector<std::string>& p_items,
                              std::vector<std::string>& p_colliders)
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
            p_colliders.clear();
            p_items.erase(comboItemToDelete);
            m_currentObject = "";
            m_currentCollider = "";
            m_objectElementSize = l_objects.size();
        }
    }
    if (ImGui::Button("Delete Object") or Inputs::InputManager::getInstance().getKeyDown(GLFW_KEY_DELETE))
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
    if (ImGui::BeginCombo("##shaderRefresh", m_currentShader.c_str()))
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

void GuiManager::choosingObjectToAddComboBox(std::vector<std::string>& p_objectList)
{
    if (ImGui::BeginCombo("##objectToAddCombo", m_currentObjectToAdd.c_str()))
    {
        for (int i = 0; i < p_objectList.size(); i++)
        {
            bool is_selected = (m_currentObjectToAdd == p_objectList.at(i));
            if (ImGui::Selectable(p_objectList.at(i).c_str(), is_selected))
            {
                m_currentObjectToAdd = p_objectList.at(i);
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

void GuiManager::updateNormalTextureComboBox(std::vector<std::string>& p_textureItems)
{
    if (ImGui::BeginCombo("##normalTextureCombo", m_cuurentNormalTexture.c_str()))
    {
        for (int i = 0; i < p_textureItems.size(); i++)
        {
            bool is_selected = (m_cuurentNormalTexture == p_textureItems.at(i));
            if (ImGui::Selectable(p_textureItems.at(i).c_str(), is_selected))
            {
                m_cuurentNormalTexture = p_textureItems.at(i);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void GuiManager::objectChoosingComboBox(std::vector<std::string>& p_items,
                                        std::vector<std::string>& p_colliders)
{

    if (ImGui::BeginCombo("##objectCombo", m_currentObject.c_str()))
    {
        for (auto& l_item : p_items)
        {
            bool is_selected = (m_currentObject == l_item);
            if (ImGui::Selectable(l_item.c_str(), is_selected))
            {
                m_currentCollider = "";
                p_colliders.clear();
                auto& l_objects =
                    Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager().getObjects();
                auto objIt = std::find_if(l_objects.begin(), l_objects.end(),
                    [&](auto& label)
                {
                    return m_currentObject == label.second;
                });
            
                m_currentObject = l_item;
                if(objIt != l_objects.end())
                {
                    setForce(objIt->first.m_rigidBody.m_force);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void GuiManager::colliderChoosingComboBox(std::vector<std::string>& p_items)
{
    if (p_items.size() <= 0)
    {
        return;
    }
    if (ImGui::BeginCombo("##colliderCombo", m_currentCollider.c_str()))
    {
        if(m_currentObject != "")
        {
            for (int i = 0; i < p_items.size(); i++)
            {
                bool is_selected = (m_currentCollider == p_items.at(i));
                if (ImGui::Selectable(p_items.at(i).c_str(), is_selected))
                {
                    m_currentCollider = p_items.at(i);
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
            }
        
            ImGui::EndCombo();
        }
    }
}

void GuiManager::colliderTypeChoosingComboBox(std::vector<std::string>& p_items)
{

    if (ImGui::BeginCombo("##colliderTypeCombo", m_colliderTypeName.c_str()))
    {
        for (int i = 0; i < p_items.size(); i++)
        {
            bool is_selected = (m_colliderTypeName == p_items.at(i));
            if (ImGui::Selectable(p_items.at(i).c_str(), is_selected))
            {
                m_colliderTypeName = p_items.at(i);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    if (m_colliderTypeName == "OBB")
    {
        m_colliderType = Meshes::ColliderType::CUBE_OBB;
    }
    else if (m_colliderTypeName == "AABB")
    {
        m_colliderType = Meshes::ColliderType::CUBE_AABB;
    }
    else if (m_colliderTypeName == "SPHERE")
    {
        m_colliderType = Meshes::ColliderType::SPHERE;
    }
}

void GuiManager::choosingObjectMaterialType(std::vector<std::string>& p_materialTypes)
{
    if (ImGui::BeginCombo("##materialTypeCombo", m_materialType.c_str()))
    {
        for (int i = 0; i < p_materialTypes.size(); i++)
        {
            bool is_selected = (m_materialType == p_materialTypes.at(i));
            if (ImGui::Selectable(p_materialTypes.at(i).c_str(), is_selected))
            {
                m_materialType = p_materialTypes.at(i);
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void GuiManager::updateObjectMassProperties()
{
    auto& l_objects =
        Scenes::ScenesManager::getInstace().getCurrentAvaiableScene()->getObjectManager().getObjects();
    if (ImGui::Button("Update MaterialType"))
    {
        auto objIt = std::find_if(l_objects.begin(), l_objects.end(),
            [&](auto& label)
        {
            return m_currentObject == label.second;
        });
        if (objIt != l_objects.end())
        {
            if (m_materialType != "")
            {
                objIt->first.m_rigidBody.m_materialProperties.setMaterialType(m_materialType);
                Physics::PhysicsManager::getInstance().calculateObjectMass(objIt->first);
            }
        }
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
                objIt->first.m_currentActiveShader = m_currentShader;
            }
            catch (std::exception& ex)
            {
                LOG("Cannot find shader, there is no shader with this name", LogType::ERR);
            }
        }
    }
}

void GuiManager::setForce(glm::vec3& p_force)
{
    m_force = &p_force;
}

void GuiManager::refreshShader()
{
    if (ImGui::Button("Refresh Shader"))
    {
        try
        {
            Shaders::ShaderManager::getInstance().getShader(m_currentShader)->refresh();
        }
        catch (std::exception& ex)
        {
            LOG("Cannot find shader, there is no shader with this name", LogType::ERR);
        }
    }
}

}//Gui
}//McEngine
