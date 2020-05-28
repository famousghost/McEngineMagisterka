#include "ObjectManager.h"
#include "TextureManager.h"
#include "LoadedObjectBuilder.h"
#include "PhysicsManager.h"
#include "ColiderObserver.h"
#include "InputManager.h"
#include <algorithm>
#include <iostream>

namespace McEngine
{
namespace Meshes
{

void ObjectManager::addObject(const Object& p_object, std::string p_objName)
{
    if (std::find_if(m_objects.begin(), m_objects.end(), 
        [&](auto& object){return object.second == p_objName;}) != m_objects.end())
    {
        LOG("Cannot add object with the same name", LogType::WARN);
        return;
    }

    m_objects.push_back(std::make_pair(p_object, p_objName));
    ColiderObserver* l_coliderObserver = new ColiderObserver(m_objects.back().first);
    m_coliderObserver.push_back(l_coliderObserver);
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

void ObjectManager::update(Object& p_object)
{
    setMaterialForObjectObject(p_object);
    setModelMatrixForObject(p_object);
    activeTextures(p_object);
}

void ObjectManager::updateCollider(Object& p_object, 
                                   Cameras::Camera& p_camera)
{
    auto& l_collider = p_object.m_colider;
    l_collider.m_shaderProgram->uniformVec3(l_collider.m_coliderColor, "coliderColor");
    glm::mat4 l_colliderModel;
    glm::mat4 l_normalsRotationeModel;
    l_colliderModel = glm::translate(l_colliderModel, p_object.m_transform.m_position);
    l_colliderModel = glm::scale(l_colliderModel, p_object.m_transform.m_scale);
    l_colliderModel = glm::rotate(l_colliderModel, glm::radians(p_object.m_transform.m_rotatione.x), glm::vec3(1.0f, 0.0f, 0.0f));
    l_colliderModel = glm::rotate(l_colliderModel, glm::radians(p_object.m_transform.m_rotatione.y), glm::vec3(0.0f, 1.0f, 0.0f));
    l_colliderModel = glm::rotate(l_colliderModel, glm::radians(p_object.m_transform.m_rotatione.z), glm::vec3(0.0f, 0.0f, 1.0f));

    l_normalsRotationeModel = glm::rotate(l_normalsRotationeModel, glm::radians(p_object.m_transform.m_rotatione.x), glm::vec3(1.0f, 0.0f, 0.0f));
    l_normalsRotationeModel = glm::rotate(l_normalsRotationeModel, glm::radians(p_object.m_transform.m_rotatione.y), glm::vec3(0.0f, 1.0f, 0.0f));
    l_normalsRotationeModel = glm::rotate(l_normalsRotationeModel, glm::radians(p_object.m_transform.m_rotatione.z), glm::vec3(0.0f, 0.0f, 1.0f));
    l_collider.m_normals.x = l_normalsRotationeModel * l_collider.m_normals.x;
    l_collider.m_normals.y = l_normalsRotationeModel * l_collider.m_normals.y;
    l_collider.m_normals.z = l_normalsRotationeModel * l_collider.m_normals.z;

    //TODO rid of this part, created only for debuging
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (m_x != l_collider.m_normals.x || m_y != l_collider.m_normals.y || m_z != l_collider.m_normals.z)
    {
        std::cout << "x: ";
        print(l_collider.m_normals.x);
        std::cout << std::endl << "y: ";
        print(l_collider.m_normals.y);
        std::cout << std::endl << "z: ";
        print(l_collider.m_normals.z);
    }
    m_x = l_collider.m_normals.x;
    m_y = l_collider.m_normals.y;
    m_z = l_collider.m_normals.z;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    transformCollider(l_collider, l_colliderModel);
    l_collider.m_shaderProgram->uniformMatrix4(l_colliderModel, "model");
    p_camera.update(*l_collider.m_shaderProgram, "cameraPos", "view", "projection");
}

void ObjectManager::transformCollider(Colider& p_collider, const glm::mat4& p_colliderModel)
{
    p_collider.reset();
    p_collider.m_minVertex = p_colliderModel * p_collider.m_minVertex;
    p_collider.m_maxVertex = p_colliderModel * p_collider.m_maxVertex;

    p_collider.m_xSection.min = p_colliderModel * p_collider.m_xSection.min;
    p_collider.m_xSection.max = p_colliderModel * p_collider.m_xSection.max;

    p_collider.m_ySection.min = p_colliderModel * p_collider.m_ySection.min;
    p_collider.m_ySection.max = p_colliderModel * p_collider.m_ySection.max;

    p_collider.m_zSection.min = p_colliderModel * p_collider.m_zSection.min;
    p_collider.m_zSection.max = p_colliderModel * p_collider.m_zSection.max;
}

void ObjectManager::print(const glm::vec4& p_vec)
{
    //TODO Rid of this function, created only for debuging
    std::cout << "(" << p_vec.x << ", " << p_vec.y << ", " << p_vec.z << ")" << std::endl;
}

void ObjectManager::setModelMatrixForObject(Object& p_object)
{
    auto& l_physcis = Physics::PhysicsManager::getInstance();
    glm::mat4 l_model;
    auto& l_transform = p_object.m_transform;
    l_model = glm::translate(l_model, l_transform.m_position);
    l_model = glm::scale(l_model, l_transform.m_scale);
    l_model = glm::rotate(l_model, glm::radians(l_transform.m_rotatione.x), glm::vec3(1.0f, 0.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(l_transform.m_rotatione.y), glm::vec3(0.0f, 1.0f, 0.0f));
    l_model = glm::rotate(l_model, glm::radians(l_transform.m_rotatione.z), glm::vec3(0.0f, 0.0f, 1.0f));
    p_object.updateTransformation(l_transform);
    l_physcis.checkCollisionDetectionAABB(p_object, m_objects);
    p_object.m_shaderProgram->uniformMatrix4(l_model, "model");
}

void ObjectManager::setMaterialForObjectObject(Object& p_object)
{
    p_object.m_shaderProgram->uniformVec3(m_lightPosition, "lightPosition");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_ambientLight, "material.ambient");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_diffuseLight, "material.diffuse");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_specularLight, "material.specular");
    p_object.m_shaderProgram->uniformVec3(p_object.m_material.m_objectColor, "material.objectColor");
}

void ObjectManager::setTexture(Mesh& p_mesh,
                               const Texture& p_texture)
{
    p_mesh.m_textures.push_back(p_texture);
}

void ObjectManager::activeTextures(Object& p_object)
{
    auto& textureManager = Textures::TextureManager::getInstance();
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

std::vector<std::pair<Object, std::string>>& ObjectManager::getObjects()
{
    return m_objects;
}

}//Meshes
}//GameEngine