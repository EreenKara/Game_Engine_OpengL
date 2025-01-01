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
#include <iostream>

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
void Scene::reset()
{
    for (auto element : this->playableObjects) {
        delete element; // Bellek alanını serbest bırak
    }
    this->playableObjects.clear();

    for (auto element : this->m_objects) {
        delete element; // Bellek alanını serbest bırak
    }
    this->m_objects.clear();
    this->activePlayableObject=nullptr;
    this->topCamera=nullptr;
    this->activeObject=nullptr;

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
    for (size_t i = 0; i < this->playableObjects.size(); i++)
    {
        if(this->activePlayableObject->getId() != this->playableObjects.at(i)->getId())
        {
            this->drawObject(this->playableObjects.at(i),this->activePlayableObject);
        }
    }
    

    
    glClear(GL_DEPTH_BUFFER_BIT); 

    glViewport(Settings::getScreenWidth() - Settings::getScreenTopCameraWidth(), Settings::getScreenHeight() - Settings::getScreenTopCameraHeight(), Settings::getScreenTopCameraWidth(), Settings::getScreenTopCameraHeight());
    this->drawObject(this->cursor,this->topCamera);
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        this->drawObject(m_objects.at(i),this->topCamera);
    }
    for (size_t i = 0; i < this->playableObjects.size(); i++)
    {
        if(this->topCamera->getId() != this->playableObjects.at(i)->getId())
        {
            this->drawObject(this->playableObjects.at(i),this->topCamera);
        }
    }
    
}
void Scene::mouseFunction(double x, double y){
    this->activePlayableObject->mouseFunction(x,y);

}
void Scene::imguiRenderFunction(){
        //-----------------------
    // İLK PANEL
    //------------------------

    ImGui::Begin("Model Control");

    static int modelId = 0;  
    int totalModels = static_cast<int>(m_objects.size());     
    WorldObject* activeObject = getActiveObject();

    //------------------
    // AKTİF OBJE SEÇİMİ
    //------------------

    ImGui::Separator();
    ImGui::Text("Model Selection");
    ImGui::Text(" ");
    if (ImGui::SliderInt("Model ID", &modelId, 0, totalModels - 1)) {
        
        if (modelId >= 0 && modelId < totalModels) {
            setActiveObject(m_objects[modelId]);
            activeObject = getActiveObject(); 
        }
    }

      static std::vector<std::string> shapes = 
        {
            "Circle","Square","Cube",  "Frustum","Pyramid",   "Cylinder"
        };

        static std::vector<std::string> textures = { "container", "cotton" ,"wood" , "default"}; 

    if (activeObject)
    {
        
        graf::ShapeTypes currentShapeType = activeObject->getShapeType();
        
        std::string currentShapeName;
        switch (currentShapeType)
        {
        case graf::ShapeTypes::Cube:
            currentShapeName = "Cube";
            break;
        case graf::ShapeTypes::Circle:
            currentShapeName = "Circle";
            break;
        case graf::ShapeTypes::Pyramid:
            currentShapeName = "Pyramid";
            break;
        case graf::ShapeTypes::Square:
            currentShapeName = "Square";
            break;
        case graf::ShapeTypes::Frustum:
            currentShapeName = "Frustum";
            break;
        case graf::ShapeTypes::Cylinder:
            currentShapeName = "Cylinder";
            break;
        }

       
      

        
        static std::string selectedShape = currentShapeName;

        //------------------
        // SHAPE SEÇİMİ
        //------------------


        if (ImGui::BeginCombo("Shape", selectedShape.c_str()))
        {
            for (size_t i = 0; i < shapes.size(); ++i)
            {
                bool isSelected = (selectedShape == shapes[i]);
                if (ImGui::Selectable(shapes[i].c_str(), isSelected))
                {
                    selectedShape = shapes[i];

                    
                    if      (selectedShape == "Cube")     activeObject->changeShape(graf::ShapeTypes::Cube);
                    else if (selectedShape == "Circle")   activeObject->changeShape(graf::ShapeTypes::Circle);
                    else if (selectedShape == "Pyramid")  activeObject->changeShape(graf::ShapeTypes::Pyramid);
                    else if (selectedShape == "Square")   activeObject->changeShape(graf::ShapeTypes::Square);
                    else if (selectedShape == "Frustum")  activeObject->changeShape(graf::ShapeTypes::Frustum);
                    else                                   activeObject->changeShape(graf::ShapeTypes::Cylinder);
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        //------------------
        // TEXTURE SEÇİMİ
        //------------------

        
        static const char* currentTexture = textures[0].c_str(); 
        static glm::vec2 textureRepeat = glm::vec2(1.0f, 1.0f);    

        ImGui::Separator();
        ImGui::Text("Textures");
        ImGui::Text(" ");

        if (ImGui::BeginCombo("Texture", currentTexture))
        {
            for (size_t i = 0; i < textures.size(); ++i)
            {
                bool isSelected = (currentTexture == textures[i].c_str());
                if (ImGui::Selectable(textures[i].c_str(), isSelected))
                {
                    currentTexture = textures[i].c_str(); 
                    activeObject->changeTexture(currentTexture); 
                }
                if (isSelected)
                 ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        //------------------
        // TEXTURE REPEAT 
        //------------------
        if (ImGui::SliderFloat2("Repeat", &activeObject->getTextureRepeat().x, 0.1f, 10.0f)) {
        activeObject->setTextureRepeat(activeObject->getTextureRepeat());
        }

        //------------------
        // OBJE TRANSFORM
        //------------------

        glm::vec3 position = activeObject->getTranform()->getPosition();
        glm::vec3 rotation = activeObject->getTranform()->getEuler();  
        glm::vec3 scale    = activeObject->getTranform()->getScale();

        ImGui::Separator();       
        ImGui::Text("Transform");
        ImGui::Text(" ");
        
       if (ImGui::SliderFloat3("Position", &position.x, -10.0f, 10.0f))
        {        
            activeObject->getTranform()->setPosition(position);
        }       

      
       if (ImGui::SliderFloat3("Rotation", &rotation.x, -100.0f, 100.0f))
        {
            activeObject->getTranform()->setEuler(rotation);
        }

        
        if (ImGui::SliderFloat3("Scale", &scale.x, -10.0f, 10.0f))
        {
            activeObject->getTranform()->setScale(scale);
        }
        }
        else
        {
        ImGui::Text("No active object selected.");
        }

        if (ImGui::Button("SAVE")) {
         Save::saveToFileAsJson();
        }
        //------------------
        // KAMERA SEÇİMİ
        //------------------

        ImGui::Separator();
        ImGui::Text("Camera Selection");
        ImGui::Text(" ");
        static int activeCameraIndex = 0;
        if (ImGui::SliderInt("Active Camera", &activeCameraIndex, 0, static_cast<int>(playableObjects.size()) - 1)) {
        setActivePlayableObject(playableObjects[activeCameraIndex]);
        }

        PlayableObject* activeCamera = getActivePlayableObject();
        if (activeCamera) {
        graf::Camera* camera = activeCamera->getCamera();

        //------------------
        // KAMERA TRANSFORM
        //------------------

        ImGui::Separator();
        ImGui::Text("Camera Transform");
        ImGui::Text(" ");
        glm::vec3 position = camera->getTransform()->getPosition();
        if (ImGui::SliderFloat3("Pos", &position.x, -50.0f, 50.0f)) {
            camera->getTransform()->setPosition(position);
        }

        
        glm::vec3 euler = camera->getTransform()->getEuler();
        if (ImGui::SliderFloat3("Rot", &euler.x, -180.0f, 180.0f)) {
            camera->getTransform()->setEuler(euler);
        }

        
        ImGui::Separator();
        ImGui::Text("Model");
        ImGui::Text(" ");
        float fov = camera->getFov();
        if (ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f)) {
            camera->setFov(fov);
        }
        float aspect = camera->getAspect();
        if (ImGui::SliderFloat("Aspect", &aspect, 1.0f, 3.0f)) {
            camera->setAspect(aspect);
        }
        float near = camera->getNear();
        if (ImGui::SliderFloat("Near Plane", &near, 0.1f, 10.0f)) {
            camera->setNear(near);
        }
        float far = camera->getFar();
        if (ImGui::SliderFloat("Far Plane", &far, 10.0f, 300.0f)) {
            camera->setFar(far);
        }
    }

    
    ImGui::End();

    //-----------------------
    //İKİNCİ PANEL 
    //----------------------

    ImGui :: Begin("Creation");


    ImGui::Separator();
    ImGui:: Text("Model Creation");
    ImGui:: Text(" ");

    static int selectedShapeIndex = 0; 
    static int selectedTextureIndex = 0; 
    static glm::vec2 textureRepeat = glm::vec2(1.0f, 1.0f); 

     ImGui::Text("Shapes");
    if (ImGui::BeginCombo("Shapes", shapes[selectedShapeIndex].c_str())) {
        for (int i = 0; i < shapes.size(); ++i) {
            bool isSelected = (selectedShapeIndex == i);
            if (ImGui::Selectable(shapes[i].c_str(), isSelected)) {
                selectedShapeIndex = i; // Seçilen Shape Index'i güncelle
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus(); // Varsayılan odak
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Text("Textures");
    if (ImGui::BeginCombo("Textures", textures[selectedTextureIndex].c_str())) {
        for (int i = 0; i < textures.size(); ++i) {
            bool isSelected = (selectedTextureIndex == i);
            if (ImGui::Selectable(textures[i].c_str(), isSelected)) {
                selectedTextureIndex = i; // Seçilen Texture Index'i güncelle
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus(); // Varsayılan odak
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Text("Repeat");
    ImGui::SliderFloat2("Repeat", &textureRepeat.x, 0.1f, 10.0f);


if (ImGui::Button("Create Model")) {
    try {
        graf::ShapeTypes selectedShape = static_cast<graf::ShapeTypes>(selectedShapeIndex);
        std::string selectedTexture = textures[selectedTextureIndex];


         WorldObject* newObject = new WorldObject(
            -1,
            selectedTexture,
            selectedShape,
            "TextureShader",
            1,
            textureRepeat
        );

        // Default transform ayarları
        newObject->getTranform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        newObject->getTranform()->setScale(glm::vec3(1.0f));

               
        // Sahneye ekleme
        addObject(newObject);
        std::cout << "Object added to the scene.  "  << std::endl;
        setActiveObject(newObject);
        // Kamera kontrolü
        if (!activeCamera) {
            std::cerr << "Error: Active camera is null!" << std::endl;
            return;
        }
        modelId = static_cast<int>(m_objects.size())-1;

        addObject(newObject);
        
        // Çizim
        drawObject(newObject, activeCamera);

        ImGui::Text("Model Created!");
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}
    ImGui::Separator();
    ImGui:: Text("Camera Creation");
    ImGui:: Text(" ");

    {
        
        static float fov = 90.0f;
        if (ImGui::SliderFloat("FOV", &fov, 30.0f, 120.0f)) {
            
        }
        
        static float near = 1.0f;
        if (ImGui::SliderFloat("Near Plane", &near, 0.1f, 10.0f)) {
            // camera->setNear(near);
        }
        static float far = 100.0f;
        if (ImGui::SliderFloat("Far Plane", &far, 10.0f, 300.0f)) {
        }

        static float width = 1920.0f;
        static float height = 1080.0f;

        if (ImGui::SliderFloat("Width", &width, 10.0f, 1920.0f)) ;
        if (ImGui::SliderFloat("Height", &height, 10.0f, 1080.0f)) ;

       
        float aspect = width / height;
    if (ImGui::Button("Create Camera")){

        PlayableObject* po = new PlayableObject(new WorldObject());
        po->setCamera(new graf::Camera(fov,aspect,near,far));
        this->addPlayableObject(po);
    }
       
        
    }
    ImGui::End();
}
void Scene::keyboardFunction(int key,int scancode,int action){
    this->activePlayableObject->keyboardFunction(key,scancode,action);
    float angle = 5.0f;
    if(true){
        if(key==GLFW_KEY_0 ) ;
        else if(key==GLFW_KEY_1) ;

        if(key==GLFW_KEY_T && action==GLFW_PRESS)
        {
            auto temp = this->activePlayableObject;
            this->activePlayableObject = this->topCamera;
            this->topCamera = temp;
        }
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