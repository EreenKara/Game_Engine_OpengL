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
#include "settings.hpp"
#include "idcounter.hpp"

Scene::Scene(){
    srand(time(0));
    this->cursor = new Cursor(new WorldObject(0,"container",graf::ShapeTypes::Pyramid,"WhiteShader"));


    this->activePlayableObject = new PlayableObject(new WorldObject(-1,"cotton"));
    float aspect = (float)Settings::getScreenWidth()/Settings::getScreenHeight();
    this->activePlayableObject->getCamera()->setAspect(aspect);
    
    this->topCamera = new PlayableObject(new WorldObject(-1,"concrete"));
    aspect = (float)Settings::getScreenTopCameraWidth()/Settings::getScreenTopCameraHeight();
    this->topCamera->getCamera()->setAspect(aspect);

    playableObjects.push_back(activePlayableObject);
    playableObjects.push_back(topCamera);
    


    glm::vec3 position(0,0,4.0f);
    glm::vec3 position2(2,0,4.0f);
    
    this->activeObject = new WorldObject(-1,"wall");
    this->addObject(this->activeObject);
    this->activeObject->getTranform()->setPosition(position2);
    // SİL 
    auto wo =new WorldObject(-1,"grass");
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
        // this->m_objects.push_back(wo);
    }
    this->activeObject = wo;
}
void Scene::setActivePlayableObject(PlayableObject* po)
{
    auto it = std::find(playableObjects.begin(),playableObjects.end(),po);
    if(it == playableObjects.end())
    {
        // playableObjects.push_back(po);
    }
    this->activePlayableObject = po;
}
void Scene::setTopCamera(PlayableObject* po)
{
    auto it = std::find(playableObjects.begin(),playableObjects.end(),po);
    if(it == playableObjects.end())
    {
        // playableObjects.push_back(po);
    }
    this->topCamera = po;
}

void Scene::renderFunction(){
    glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glViewport(0,0,Settings::getScreenWidth(),Settings::getScreenHeight());

    this->cursor->calculateCursorPosition(this->activeObject);
    this->drawObject(this->cursor,this->activePlayableObject);
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        this->drawObject(m_objects.at(i),this->activePlayableObject);
    }

    glClear(GL_DEPTH_BUFFER_BIT); 

    glViewport(Settings::getScreenWidth() - Settings::getScreenTopCameraWidth(), Settings::getScreenHeight() - Settings::getScreenTopCameraHeight(), Settings::getScreenTopCameraWidth(), Settings::getScreenTopCameraHeight());
    this->drawObject(this->cursor,this->topCamera);
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        this->drawObject(m_objects.at(i),this->topCamera);
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
    float angle = 5.0f;
    if(true){
        if(key==GLFW_KEY_0 ) ;
        else if(key==GLFW_KEY_1) ;

        if(key==GLFW_KEY_O && action==GLFW_PRESS)
        {
            auto scale = activeObject->getTranform()->getScale();
            scale.x +=1;
            scale.y +=1;
            scale.z +=1;
            activeObject->getTranform()->setScale(scale);
        }
        if(key==GLFW_KEY_P && action==GLFW_PRESS)
        {
            auto scale = activeObject->getTranform()->getScale();
            scale.x -=1;
            scale.y -=1;
            scale.z -=1;
            activeObject->getTranform()->setScale(scale);
        }
        if(key==GLFW_KEY_E )
        {
            auto euler = activeObject->getTranform()->getEuler();
            euler.y+=angle;
            activeObject->getTranform()->setEuler(euler);
        }
        if(key==GLFW_KEY_Q )
        {
            auto euler = activeObject->getTranform()->getEuler();
            euler.y-=angle;
            activeObject->getTranform()->setEuler(euler);
        }
        if(key==GLFW_KEY_Y )
        {
            auto euler = activeObject->getTranform()->getEuler();
            euler.x+=angle;
            activeObject->getTranform()->setEuler(euler);
        }
        if(key==GLFW_KEY_H )
        {
            auto euler = activeObject->getTranform()->getEuler();
            euler.x-=angle;
            activeObject->getTranform()->setEuler(euler);
        }
        if(key==GLFW_KEY_U )
        {
            auto euler = activeObject->getTranform()->getEuler();
            euler.z+=angle;
            activeObject->getTranform()->setEuler(euler);
        }
        if(key==GLFW_KEY_J )
        {
            auto euler = activeObject->getTranform()->getEuler();
            euler.z-=angle;
            activeObject->getTranform()->setEuler(euler);
        }
        if(key==GLFW_KEY_SPACE && action==GLFW_PRESS)
        {
            auto wo = new WorldObject(-1,"container",graf::ShapeTypes::Square,"TextureShader");
            addObject(wo);
            setActiveObject(wo);
        }
        if(key==GLFW_KEY_UP && action==GLFW_PRESS)
        {
            activeObject->getTranform()->moveUp();
        }
        else if(key == GLFW_KEY_DOWN && action==GLFW_PRESS)
        {
            activeObject->getTranform()->moveDown();
        } 
        else if(key == GLFW_KEY_RIGHT && action==GLFW_PRESS)
        {
            activeObject->getTranform()->moveRight();
        }
        else if(key == GLFW_KEY_LEFT && action==GLFW_PRESS)
        {
            activeObject->getTranform()->moveLeft();
        }
        else if(key == GLFW_KEY_M && action==GLFW_PRESS)
        {
            activeObject->getTranform()->moveForward();
        }
        else if(key == GLFW_KEY_N && action==GLFW_PRESS)
        {
            activeObject->getTranform()->moveBackward();
        }
    }
}

void Scene::drawObject(WorldObject* object,PlayableObject* playableObject){
    object->getShaderProgram()->use();
    graf::VertexArrayObject* va=graf::ShapeCreator::create(object->getShapeType());
    va->bind();
    glm::mat4 mtxWorld = object->getTranform()->getWorldMatrix();
    auto viewMatrix = playableObject->getCamera()->getViewMatrix();
    auto projectionMatrix = playableObject->getCamera()->getProjectionMatrix();
    object->getShaderProgram()->setVec2("uTextureRepeat",object->getTextureRepeat());
    object->getShaderProgram()->setMat4("uWorldTransform",projectionMatrix*viewMatrix*mtxWorld);
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