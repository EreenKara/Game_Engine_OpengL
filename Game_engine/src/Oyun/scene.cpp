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

Scene::Scene(){
    srand(time(0));
    this->playableObject = new PlayableObject();
    this->shaderProgramName = "TextureShader";

    // SİL 
    glm::vec3 position(0,0,4.0f);
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
    return;
}
void Scene::removeObject(WorldObject* worldObject)
{
    auto it = std::find(this->m_objects.begin(), this->m_objects.end(), worldObject);
    if(it != m_objects.end())
    {
        this->m_objects.erase(it);
    }
}
            

void Scene::renderFunction(){
    glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    auto shader = graf::ShaderManager::getShaderProgram(shaderProgramName);
    shader->use();
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        this->drawObject(m_objects.at(i),this->playableObject,shader);
    }
    
}
void Scene::mouseFunction(double x, double y){
    this->playableObject->mouseFunction(x,y);

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
    this->playableObject->keyboardFunction(key,scancode,action);

    if(true){
        if(key==GLFW_KEY_0 ) ;
        else if(key==GLFW_KEY_1) ;


        if(key==GLFW_KEY_SPACE && action==GLFW_PRESS)
        {
        }
        if(key==GLFW_KEY_UP)
        {
        }
        else if(key == GLFW_KEY_DOWN)
        {
        } 
        else if(key == GLFW_KEY_RIGHT)
        {
        }
        else if(key == GLFW_KEY_LEFT)
        {
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
    delete playableObject;
}
void Scene::deleteWorldObjects(){
    for(WorldObject* item : m_objects)
    {
        delete item;
    }
}