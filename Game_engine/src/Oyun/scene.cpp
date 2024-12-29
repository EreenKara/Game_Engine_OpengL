#include "scene.hpp"
#include "glwindow.hpp"
#include "worldobject.hpp"
#include "vertexarrayobject.hpp"
#include "texturemanager.hpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "genelFonksiyonlar.hpp"
#include <cstdlib> // rand ve srand için gerekli
#include <ctime>   // srand için gerekli
#include <fstream>
#include <algorithm>

Scene::Scene(std::string shaderProgramName){
    srand(time(0));
    this->activePlayableObject = new PlayableObject(new WorldObject());
    this->topCamera = new PlayableObject(new WorldObject());
    playableObjects.push_back(activePlayableObject);
    playableObjects.push_back(topCamera);
    this->shaderProgramName = "TextureShader";

    glm::vec3 position(0,0,4.0f);
    glm::vec3 position2(2,0,4.0f);
    
    this->activeObject = new WorldObject();
    this->addObject(this->activeObject);
    this->activeObject->getTranform()->setPosition(position2);
    // SİL 
    auto wo =new WorldObject();
    this->addObject(wo);
    wo->getTranform()->setPosition(position);
}
WorldObject* Scene::getObject(WorldObject* worldObject)
{
    auto it = std::find(m_objects.begin(),m_objects.end(),worldObject);
    return it != m_objects.end() ? *it: nullptr;
}
WorldObject* Scene::getObject(unsigned int id)
{
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        if(m_objects.at(i)->getId() == id) return m_objects.at(i);
    }
    return nullptr;
}
void Scene::addObject(WorldObject* worldObject)
{
    auto it = std::find(m_objects.begin(),m_objects.end(),worldObject);
    if(it == m_objects.end())
    {
        m_objects.push_back(worldObject);
    }
}
void Scene::removeObject(WorldObject* worldObject)
{
    auto it = std::find(this->m_objects.begin(), this->m_objects.end(), worldObject);
    if(it != m_objects.end())
    {
        
        delete *it;
        this->m_objects.erase(it);
    }
}
void Scene::removePlayableObject(PlayableObject* po)
{
    auto it = std::find(this->playableObjects.begin(), this->playableObjects.end(), po);
     if(it != playableObjects.end())
    {
        if(*it != activePlayableObject ||*it != topCamera )
        {
            delete *it;
            this->playableObjects.erase(it);
        }
    }
}
void Scene::addPlayableObject(PlayableObject* po)
{
    auto it = std::find(playableObjects.begin(),playableObjects.end(),po);
    if(it == playableObjects.end())
    {
        playableObjects.push_back(po);
    }
}
PlayableObject* Scene::getActivePlayableObject()
{
    return this->activePlayableObject;
}
PlayableObject* Scene::getTopCamera()
{
    return this->topCamera;
}
WorldObject* Scene::getActiveObject()
{
    return this->activeObject;
}

void Scene::setActiveObject(WorldObject* wo)
{
    auto it = std::find(this->m_objects.begin(), this->m_objects.end(), wo);
    if(it == m_objects.end())
    {
        this->m_objects.push_back(wo);
    }
    this->activeObject = wo;
}
void Scene::setActivePlayableObject(PlayableObject* po)
{
    auto it = std::find(playableObjects.begin(),playableObjects.end(),po);
    if(it == playableObjects.end())
    {
        playableObjects.push_back(po);
    }
    this->activePlayableObject = po;
}
void Scene::setTopCamera(PlayableObject* po)
{
    auto it = std::find(playableObjects.begin(),playableObjects.end(),po);
    if(it == playableObjects.end())
    {
        playableObjects.push_back(po);
    }
    this->topCamera = po;
}
void Scene::setShaderProgramName(std::string shaderProgramName)
{
    this->shaderProgramName=shaderProgramName;
}

void Scene::renderFunction(){
    glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    auto shader = graf::ShaderManager::getShaderProgram(shaderProgramName);
    shader->use();
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        this->drawObject(m_objects.at(i),this->activePlayableObject,shader);
    }
    
}
void Scene::mouseFunction(double x, double y){
    this->activePlayableObject->mouseFunction(x,y);

}
void Scene::imguiRenderFunction(){
    ImGui::Begin("Gui Window");
    ImGui::Text("Right-Handed");
    // graf::printMatrix(m_mtxProjection);
    ImGui::Text("Left-Handed");
    // graf::printMatrix(m_mtxProjection);

    
    ImGui::End();
}
void Scene::keyboardFunction(int key,int scancode,int action){
    this->activePlayableObject->keyboardFunction(key,scancode,action);

    if(true){
        if(key==GLFW_KEY_0 ) ;
        else if(key==GLFW_KEY_1) ;


        if(key==GLFW_KEY_SPACE && action==GLFW_PRESS)
        {
        }
        if(key==GLFW_KEY_UP)
        {
            activeObject->getTranform()->moveUp();
        }
        else if(key == GLFW_KEY_DOWN)
        {
            activeObject->getTranform()->moveDown();
        } 
        else if(key == GLFW_KEY_RIGHT)
        {
            activeObject->getTranform()->moveRight();
        }
        else if(key == GLFW_KEY_LEFT)
        {
            activeObject->getTranform()->moveLeft();
        }
    }
}

void Scene::drawObject(WorldObject* object,PlayableObject* playableObject, graf::ShaderProgram* shaderProgram){
    graf::VertexArrayObject* va=graf::ShapeCreator::create(object->getShapeType());
    va->bind();
    glm::mat4 mtxWorld = object->getTranform()->getWorldMatrix();
    auto viewMatrix = playableObject->getCamera()->getViewMatrix();
    auto projectionMatrix = playableObject->getCamera()->getProjectionMatrix();
    shaderProgram->setMat4("uWorldTransform",projectionMatrix*viewMatrix*mtxWorld);
    graf::TextureManager::activateTexture(object->getTextureName());
    va->draw();
    va->unbind();
    // ortak va batch renderin bakabilrisin.
}



Scene::~Scene(){
    this->deleteWorldObjects();
    for (auto &&po : this->playableObjects)
    {
        delete po;
    }
    
}
void Scene::deleteWorldObjects(){
    for(WorldObject* item : m_objects)
    {
        delete item;
    }
}