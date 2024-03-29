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
#include "PrefabManager.h"
#include "WindowManager.h"
#include <quaternion.hpp>
#include "../glm-0.9.6.3/glm/glm/gtx/quaternion.hpp"
#include "Geometry3dUtils.h"
#include <math.h>
#include <algorithm>

namespace McEngine
{
namespace Meshes
{

namespace
{
    constexpr float CONST_VELOCITY = 100.0f;
}

void ObjectManager::addObject(const Object& p_object, std::string p_objName)
{
    if (std::find_if(m_objects.begin(), m_objects.end(), 
        [&](auto& object){return object.second == p_objName;}) != m_objects.end())
    {
        LOG("Cannot add object with the same name", LogType::WARN);
        return;
    }

    auto& l_physicsManager = Physics::PhysicsManager::getInstance();
    m_objects.push_back(std::make_pair(p_object, p_objName));
    m_objects.back().first.m_objectName = p_objName;
    ColiderObserver* l_coliderObserver = new ColiderObserver(m_objects.back().first);
    m_coliderObserver.push_back(l_coliderObserver);
    auto l_materialType = Gui::GuiManager::getInstance().getMaterialType();
    m_objects.back().first.m_rigidBody.m_materialProperties.setMaterialType(l_materialType);
    Physics::PhysicsManager::getInstance().calculateObjectMass(m_objects.back().first);
    Gui::GuiManager::getInstance().setForce(m_objects.back().first.m_rigidBody.m_force);

    initState(m_objects.back().first);
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
    std::string p_shaderLabel,
    ColliderType p_defaultColliderType)
{
    TerrainBuilder l_terrainBuilder(p_defaultColliderType);
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
    resetValues(p_object);
    setMaterialForObject(p_object);
    setSpecialUniformsForObject(p_object);
    setModelMatrixForObject(p_object);
    moveObject(p_object);
    activeTextures(p_object);
}

void ObjectManager::dbgVector(const glm::vec3& p_vec, const std::string& p_msg)
{
    std::cout << p_msg << "(" << p_vec.x << ", " << p_vec.y << ", " << p_vec.z << ")" << std::endl;
}

void ObjectManager::moveObject(Object& p_object)
{
    auto& l_inputManager = Inputs::InputManager::getInstance();
    auto& l_timeManager = Time::TimeManager::getInstance();
    auto& l_physicsManager = Physics::PhysicsManager::getInstance();
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

    p_object.m_velocity = p_object.m_movementDirection * static_cast<float>(l_timeManager.getDeltaTime());

    if(p_object.m_isRigidBody)
    {
        l_physicsManager.ode(p_object);
    }

    p_object.m_transform.m_position += (p_object.m_velocity + p_object.m_rigidBody.m_velocity)  * static_cast<float>(l_timeManager.getDeltaTime());
}

void ObjectManager::resetValues(Object& p_object)
{
    p_object.m_movementDirection = glm::vec3();
    p_object.m_rigidBody.m_width = 1.0f;
    p_object.m_rigidBody.m_height = 1.0f;
    p_object.m_rigidBody.m_length = 1.0f;
}

void ObjectManager::updateCollider(Object& p_object,
    Cameras::Camera& p_camera)
{
    auto& l_rigidbody = p_object.m_rigidBody;
    for (auto& l_collider : p_object.m_colider)
    {
        l_collider.m_shaderProgram->uniformVec3(l_collider.m_coliderColor, "coliderColor");
        glm::mat4 l_colliderModel;
        glm::mat4 l_colliderModelRotatione;
        auto l_colliderTranslate = p_object.m_transform.m_position + l_collider.m_transform.m_position;
        auto l_colliderRotationeX = p_object.m_transform.m_rotatione.x + l_collider.m_transform.m_rotatione.x;
        auto l_colliderRotationeY = p_object.m_transform.m_rotatione.y + l_collider.m_transform.m_rotatione.y;
        auto l_colliderRotationeZ = p_object.m_transform.m_rotatione.z + l_collider.m_transform.m_rotatione.z;
        auto l_colliderScale = p_object.m_transform.m_scale + l_collider.m_transform.m_scale;
        l_collider.m_width *= l_colliderScale.x;
        l_collider.m_height *= l_colliderScale.y;
        l_collider.m_length *= l_colliderScale.z;
        l_collider.m_radius = l_colliderScale.x;
        l_colliderModel *= glm::toMat4(glm::normalize(l_rigidbody.m_quat));
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

        p_object.m_transform.m_orientation = glm::mat3(l_colliderModel);

        l_colliderModel = glm::scale(l_colliderModel, l_colliderScale);
        l_collider.m_shaderProgram->uniformMatrix4(l_colliderModel, "model");
        transformCollider(l_collider, l_colliderModel);
        p_camera.updateShaderProgram(*l_collider.m_shaderProgram, "cameraPos", "view", "projection");
    }
}

void ObjectManager::debugMatrix(const glm::mat4& p_mat)
{
    std::cout << "-----------------------------------" << std::endl;
    for (auto i = 0; i < 3; ++i)
    {
        for (auto j = 0; j < 3; ++j)
        {
            std::cout << "R[" << i << "]" << "[" << j << "] = " << p_mat[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
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
    float l_deltaTime = Time::TimeManager::getInstance().getDeltaTime();

    auto& l_rigidBody = p_object.m_rigidBody;
    l_model *= glm::toMat4(glm::normalize(l_rigidBody.m_quat));
    l_model = glm::translate(l_model, l_transform.m_position);
    l_model = glm::rotate(l_model, glm::radians(l_transform.m_rotatione.x), glm::vec3(1.0f, 0.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(l_transform.m_rotatione.y), glm::vec3(0.0f, 1.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(l_transform.m_rotatione.z), glm::vec3(0.0f, 0.0f, 1.0f));
    l_model = glm::scale(l_model, l_transform.m_scale);
    scaleObjectSize(p_object);
    if(p_object.m_colider.size() > 0)
    { 
        auto& l_collider = p_object.m_colider.at(0);
        auto l_colliderScale = p_object.m_transform.m_scale + l_collider.m_transform.m_scale;
        l_collider.m_width *= l_colliderScale.x;
        l_collider.m_height *= l_colliderScale.y;
        l_collider.m_length *= l_colliderScale.z;
    }
    p_object.m_modelMatrix = l_model;
    l_physcis.updatePhysics(p_object, m_objects);
    p_object.m_shaderProgram->uniformMatrix4(l_model, "model");
}

void ObjectManager::initState(Object& p_object)
{
    auto& l_physicsManager = Physics::PhysicsManager::getInstance();
    if(p_object.m_colider.size() > 0)
    {
        auto& l_collider = p_object.m_colider.at(0);
        if(l_collider.m_colliderType == Meshes::ColliderType::CUBE_OBB)
        {
            auto l_massDivision = p_object.m_rigidBody.m_massProperties.m_mass / 12.0f;
            p_object.m_rigidBody.m_bodyTensorOfInertia[0][0] = l_massDivision * std::pow(p_object.m_rigidBody.m_height, 2.0) * std::pow(p_object.m_rigidBody.m_length, 2.0);
            p_object.m_rigidBody.m_bodyTensorOfInertia[1][1] = l_massDivision * std::pow(p_object.m_rigidBody.m_width, 2.0) * std::pow(p_object.m_rigidBody.m_length, 2.0);
            p_object.m_rigidBody.m_bodyTensorOfInertia[2][2] = l_massDivision * std::pow(p_object.m_rigidBody.m_width, 2.0) * std::pow(p_object.m_rigidBody.m_height, 2.0);
        }
        else if (l_collider.m_colliderType == Meshes::ColliderType::SPHERE)
        {
            auto l_massDivision = (2.0f * p_object.m_rigidBody.m_massProperties.m_mass) / 5.0f;
            p_object.m_rigidBody.m_bodyTensorOfInertia[0][0] = l_massDivision * std::pow(l_collider.m_radius, 2.0);
            p_object.m_rigidBody.m_bodyTensorOfInertia[1][1] = l_massDivision * std::pow(l_collider.m_radius, 2.0);
            p_object.m_rigidBody.m_bodyTensorOfInertia[2][2] = l_massDivision * std::pow(l_collider.m_radius, 2.0);
        }
    }
    p_object.m_rigidBody.m_invBodyTensorOfInteria = glm::inverse(p_object.m_rigidBody.m_bodyTensorOfInertia);
    p_object.m_rigidBody.m_position = &p_object.m_transform.m_position;
}

glm::vec3 ObjectManager::getEulerAngelsFromTransform(const glm::vec3& p_rotatione)
{
    return glm::vec3(glm::radians(p_rotatione.x),
                     glm::radians(p_rotatione.y),
                     glm::radians(p_rotatione.z));
}

glm::mat4 ObjectManager::getRotationeMatrixFromEulerAngels(const glm::vec3& p_eulerAngels)
{
    return glm::toMat4(glm::quat(p_eulerAngels));
}

void ObjectManager::scaleObjectSize(Object& p_object)
{
    auto& l_transform = p_object.m_transform;
    p_object.m_rigidBody.m_width *= l_transform.m_scale.x;
    p_object.m_rigidBody.m_height *= l_transform.m_scale.y;
    p_object.m_rigidBody.m_length *= l_transform.m_scale.z;
}

void ObjectManager::setMaterialForObject(Object& p_object)
{
    p_object.m_shaderProgram->uniformVec3(m_lightPosition, "lightPosition");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_ambientLight, "material.ambient");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_diffuseLight, "material.diffuse");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_specularLight, "material.specular");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_highlightColor, "material.objectColor");
}

void ObjectManager::setSpecialUniformsForObject(Object& p_object)
{
    p_object.m_shaderProgram->uniform1f(Time::TimeManager::getInstance().getCurrentTime(), "Time");
    p_object.m_shaderProgram->uniformVec3(PrefabManager::getInstance().getTerrainCenter(), "TerrainCenter");
}

void ObjectManager::setTexture(Mesh& p_mesh,
                               const Texture& p_texture)
{
    p_mesh.m_textures.push_back(p_texture);
}

void ObjectManager::activeTextures(Object& p_object)
{
    auto& textureManager = Textures::TextureManager::getInstance();
    auto& windowManager = GameWindow::WindowManager::getInstance();
    textureManager.deactiveTexture();
    textureManager.activeTexture(GL_TEXTURE0, p_object.m_currentAvaiableTexture);
    textureManager.setTextureIdInShader(p_object.m_currentActiveShader);
    textureManager.activeNormalTexture(GL_TEXTURE1, p_object.m_currentAvaiableNormalTexture);
    textureManager.setNormalTextureIdInShader(p_object.m_currentActiveShader);
    textureManager.activeTextureById(
        GL_TEXTURE30, windowManager.getShadowMapId());
    p_object.m_shaderProgram->uniform1I(30, "shadowMap");
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

std::string ObjectManager::cloneObject(std::string p_objName)
{
    std::string l_newName = p_objName;
    auto& l_inputManager = Inputs::InputManager::getInstance();
    auto l_keys = l_inputManager.getKeyDown(GLFW_KEY_LEFT_SHIFT) * l_inputManager.getKeyDown(GLFW_KEY_C);
    if(l_keys)
    {
        auto objectToClone = std::find_if(m_objects.begin(), m_objects.end(),
            [&](auto& object)
        {
            return object.second == p_objName;
        });

        if (objectToClone == m_objects.end())
        {
            LOG("Object with this name does not exist", LogType::WARN);
            return "";
        }

        l_newName = p_objName + "(clone" + std::to_string(objectToClone->first.m_cloneNum++) + ")";
        addObject(objectToClone->first, l_newName);
        l_inputManager.setKeyUp(GLFW_KEY_LEFT_SHIFT);
    }
    return l_newName;
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