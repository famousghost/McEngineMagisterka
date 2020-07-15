#include "ObjectManager.h"
#include "TextureManager.h"
#include "LoadedObjectBuilder.h"
#include "TerrainBuilder.h"
#include "SkyBoxBuilder.h"
#include "PhysicsManager.h"
#include "ColiderObserver.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "GuiManager.h"
#include <algorithm>

namespace McEngine
{
namespace Meshes
{

namespace
{
    constexpr float CONST_VELOCITY = 1.0f;
}

void ObjectManager::addObject(const Object& p_object, std::string p_objName)
{
    if (std::find_if(m_objects.begin(), m_objects.end(), 
        [&](auto& object){return object.second == p_objName;}) != m_objects.end())
    {
        LOG("Cannot add object with the same name", LogType::WARN);
        return;
    }

    m_objects.push_back(std::make_pair(p_object, p_objName));
    m_objects.back().first.m_objectName = p_objName;
    ColiderObserver* l_coliderObserver = new ColiderObserver(m_objects.back().first);
    m_coliderObserver.push_back(l_coliderObserver);
}

void ObjectManager::addCustomObject(std::string p_objectLabel,
                                    std::string p_objectName,
                                    std::string p_shaderLabel,
                                    ColliderType p_defaultColliderType)
{
    LoadedObjectBuilder l_loadedObjectBuilder(p_objectName, p_objectLabel, p_defaultColliderType);
    l_loadedObjectBuilder.addShaderProgram(p_shaderLabel).addMesh();
    auto l_object = l_loadedObjectBuilder.getObject();
    addObject(l_object, p_objectLabel);
}

void ObjectManager::addCustomObject(std::string p_objectLabel,
    std::string p_objectName,
    std::string p_shaderLabel)
{
    LoadedObjectBuilder l_loadedObjectBuilder(p_objectName, p_objectLabel);
    l_loadedObjectBuilder.addShaderProgram(p_shaderLabel).addMesh();
    auto l_object = l_loadedObjectBuilder.getObject();
    addObject(l_object, p_objectLabel);
}

void ObjectManager::addTerrain(std::string p_objectLabel,
    std::string p_shaderLabel)
{
    TerrainBuilder l_terrainBuilder;
    l_terrainBuilder.addShaderProgram(p_shaderLabel).addMesh();
    auto l_object = l_terrainBuilder.getObject();
    addObject(l_object, p_objectLabel);
}

void ObjectManager::addSkyBox()
{
    SkyBoxBuilder l_skyBoxBuilder;
    l_skyBoxBuilder.addShaderProgram("skyBoxShader").addMesh();
    m_skyBox = l_skyBoxBuilder.getObject();

    m_skyBox.m_shaderProgram->bindShaderProgram();
    m_skyBox.m_shaderProgram->uniform1I(0, "cubemap");
    m_skyBox.m_shaderProgram->unbindShaderProgram();
}

void ObjectManager::update(Object& p_object)
{
    setMaterialForObjectObject(p_object);
    moveObject(p_object);
    if (p_object.m_isRigidBody && p_object.m_gravityForce)
    {
        gravity(p_object);
    }
    setModelMatrixForObject(p_object);
    activeTextures(p_object);
}

void ObjectManager::gravity(Object& p_object)
{
    auto& l_timeManager = Time::TimeManager::getInstance();

    if(not p_object.m_isColliding)
    {
        p_object.m_velocity.y -= (p_object.m_gravity / p_object.m_rigidBody.m_mass) * l_timeManager.getDeltaTime();
    }
}

void ObjectManager::moveObject(Object& p_object)
{
    auto& l_inputManager = Inputs::InputManager::getInstance();
    auto& l_timeManager = Time::TimeManager::getInstance();
    p_object.m_movementDirection = glm::vec3();
    if(p_object.m_objectName == Gui::GuiManager::getInstance().getCurrentAviableObject())
    {
        if (l_inputManager.getKeyDown(GLFW_KEY_LEFT))
        {
            p_object.m_movementDirection.x = -CONST_VELOCITY;
        }
        if (l_inputManager.getKeyDown(GLFW_KEY_RIGHT))
        {
            p_object.m_movementDirection.x = CONST_VELOCITY;
        }
        if (l_inputManager.getKeyDown(GLFW_KEY_UP))
        {
            p_object.m_movementDirection.z = -CONST_VELOCITY;
        }
        if (l_inputManager.getKeyDown(GLFW_KEY_DOWN))
        {
            p_object.m_movementDirection.z = CONST_VELOCITY;
        }
        if (l_inputManager.getKeyDown(GLFW_KEY_U))
        {
            p_object.m_movementDirection.y = CONST_VELOCITY;
        }
        if (l_inputManager.getKeyDown(GLFW_KEY_J))
        {
            p_object.m_movementDirection.y = -CONST_VELOCITY;
        }
    }

    auto l_move = (p_object.m_movementDirection + p_object.m_velocity) * static_cast<float>(l_timeManager.getDeltaTime());

    if(not p_object.m_isColliding)
    {
        p_object.m_transform.m_position += l_move;
    }
}

void ObjectManager::updateCollider(Object& p_object,
    Cameras::Camera& p_camera)
{
    for (auto& l_collider : p_object.m_colider)
    {
        l_collider.m_shaderProgram->uniformVec3(l_collider.m_coliderColor, "coliderColor");
        glm::mat4 l_colliderModel;
        glm::mat4 l_colliderModelRotatione;
        auto l_colliderTranslate = p_object.m_transform.m_position + l_collider.m_transform.m_position;
        auto l_colliderRotationeX = p_object.m_transform.m_rotatione.x + l_collider.m_transform.m_rotatione.x;
        auto l_colliderRotationeY = p_object.m_transform.m_rotatione.y + l_collider.m_transform.m_rotatione.y;
        auto l_colliderRotationeZ = p_object.m_transform.m_rotatione.z + l_collider.m_transform.m_rotatione.z;
        auto l_colliderSacale = p_object.m_transform.m_scale + l_collider.m_transform.m_scale;
        l_collider.m_radius = l_colliderSacale.x;
        l_colliderModel = glm::translate(l_colliderModel, l_colliderTranslate);
        l_colliderModel = glm::rotate(l_colliderModel,
            glm::radians(l_colliderRotationeX),
            glm::vec3(1.0f, 0.0f, 0.0f));
        l_colliderModel = glm::rotate(l_colliderModel,
            glm::radians(l_colliderRotationeY),
            glm::vec3(0.0f, 1.0f, 0.0f));
        l_colliderModel = glm::rotate(l_colliderModel,
            glm::radians(l_colliderRotationeZ),
            glm::vec3(0.0f, 0.0f, 1.0f));
        l_colliderModel = glm::scale(l_colliderModel, l_colliderSacale);

        l_collider.m_shaderProgram->uniformMatrix4(l_colliderModel, "model");
        transformCollider(l_collider, l_colliderModel);
        p_camera.updateShaderProgram(*l_collider.m_shaderProgram, "cameraPos", "view", "projection");
    }
}

void ObjectManager::transformCollider(Collider& p_collider, const glm::mat4& p_colliderModel)
{
    p_collider.setDefaultValues();
    p_collider.m_modelMatrix = p_colliderModel;
    p_collider.m_minVertex = p_colliderModel * p_collider.m_minVertex;
    p_collider.m_maxVertex = p_colliderModel * p_collider.m_maxVertex;

    p_collider.m_xSection.min = p_colliderModel * p_collider.m_xSection.min;
    p_collider.m_xSection.max = p_colliderModel * p_collider.m_xSection.max;

    p_collider.m_ySection.min = p_colliderModel * p_collider.m_ySection.min;
    p_collider.m_ySection.max = p_colliderModel * p_collider.m_ySection.max;

    p_collider.m_zSection.min = p_colliderModel * p_collider.m_zSection.min;
    p_collider.m_zSection.max = p_colliderModel * p_collider.m_zSection.max;

    auto modelITN = glm::mat3(glm::transpose(glm::inverse(p_colliderModel)));
    p_collider.m_normals.x = modelITN * p_collider.m_normals.x;
    p_collider.m_normals.y = modelITN * p_collider.m_normals.y;
    p_collider.m_normals.z = modelITN * p_collider.m_normals.z;

    for (auto& elem : p_collider.m_verticies)
    {
        elem = p_colliderModel * elem;
    }
}

void ObjectManager::setModelMatrixForObject(Object& p_object)
{
    auto& l_physcis = Physics::PhysicsManager::getInstance();
    glm::mat4 l_model;
    auto& l_transform = p_object.m_transform;
    l_model = glm::translate(l_model, l_transform.m_position);
    l_model = glm::rotate(l_model, glm::radians(l_transform.m_rotatione.x), glm::vec3(1.0f, 0.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(l_transform.m_rotatione.y), glm::vec3(0.0f, 1.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(l_transform.m_rotatione.z), glm::vec3(0.0f, 0.0f, 1.0f));
    l_model = glm::scale(l_model, l_transform.m_scale);
    p_object.updateTransformation(l_transform);
    l_physcis.collisionChecker(p_object, m_objects);
    p_object.m_shaderProgram->uniformMatrix4(l_model, "model");
}

void ObjectManager::setMaterialForObjectObject(Object& p_object)
{
    p_object.m_shaderProgram->uniformVec3(m_lightPosition, "lightPosition");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_ambientLight, "material.ambient");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_diffuseLight, "material.diffuse");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_specularLight, "material.specular");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_highlightColor, "material.objectColor");
}

void ObjectManager::setTexture(Mesh& p_mesh,
                               const Texture& p_texture)
{
    p_mesh.m_textures.push_back(p_texture);
}

void ObjectManager::activeTextures(Object& p_object)
{
    auto& textureManager = Textures::TextureManager::getInstance();
    textureManager.deactiveTexture();
    textureManager.setTextureIdInShader("textureShader");
    textureManager.activeTexture(GL_TEXTURE0, p_object.m_currentAvaiableTexture);
}

void ObjectManager::deleteObject(std::string p_objName)
{
    auto objectToDeleteIt = std::find_if(m_objects.begin(), m_objects.end(),
    [&](auto& object) 
    {
        return object.second == p_objName; 
    });

    if (objectToDeleteIt == m_objects.end())
    {
        LOG("Object with this name does not exist", LogType::WARN);
        return;
    }
    m_objects.erase(objectToDeleteIt);
}

Object& ObjectManager::getSkybox()
{
    return m_skyBox;
}

std::vector<std::pair<Object, std::string>>& ObjectManager::getObjects()
{
    return m_objects;
}

}//Meshes
}//GameEngine