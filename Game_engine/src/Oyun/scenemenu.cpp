/*
    Scene için partial class olarak düşünebilirsin.

*/


#include "scene.hpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "genelFonksiyonlar.hpp"
#include <iostream>

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