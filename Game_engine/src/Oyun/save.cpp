#include "save.hpp"
#include <fstream>
#include "idcounter.hpp"
#include <vector>
#include <iostream>



Save* Save::m_Ins = nullptr;
using namespace nlohmann;
Save::Save()
{
}
Save* Save::getInstance()
{
    if(m_Ins==nullptr)
    {
        m_Ins = new Save();
    }

    return m_Ins;
}


void Save::setSceneToSave(Scene* scene)
{
    auto instance = getInstance();
    instance->scene =scene;
}
bool Save::isThereReadableSaveFile()
{
    auto instance = getInstance();
    auto fileName = instance->m_saveFileName;
    std::ifstream file("./save_files/"+fileName);
    bool readable = file ? true:false;
    file.close();
    return readable;
}
void Save::deleteInstance(){
    if(getInstance() != nullptr)
        delete getInstance();
}
Save::~Save()
{
}

void Save::saveToFileAsJson()
{
    auto manager = getInstance();
    std::string fileName = manager->m_saveFileName;
    std::ofstream ofstreamFile("./save_files/"+fileName);
    json jsonData;
    jsonData = saveSceneAsJson();
    ofstreamFile << jsonData.dump(4);
    ofstreamFile.close();
}

json Save::saveSceneAsJson()
{
    auto manager = getInstance();
    auto scene = manager->scene;
    json sceneObject = {
        {"activePlayableObject",scene->activePlayableObject->id },
        {"topCamera", scene->topCamera->id},
        {"playableObjects", json::array()},
        {"activeObject", scene->activeObject->id},
        {"objects", json::array()}
    };
    
    for (int i = 0; i < manager->scene->playableObjects.size(); i++)
    {
        sceneObject["playableObjects"].push_back(savePlayableObjectAsJson(manager->scene->playableObjects.at(i)));
    }
    for (int i = 0; i < manager->scene->m_objects.size(); i++)
    {
        sceneObject["objects"].push_back(saveWorldObjectAsJson(manager->scene->m_objects.at(i)));
    }
    std::cout<<"save scene object size:"<<scene->m_objects.size()<<std::endl;
    std::cout<<"save scene playable objectsize :"<<scene->playableObjects.size()<<std::endl;

    return sceneObject;
}
json Save::saveWorldObjectAsJson(WorldObject* wo)
{
    json worldObjectJson = {
        {"id", wo->id},
        {"transform", saveTransformAsJson(wo->transform)},
        {"shapeType", static_cast<int>(wo->shapeType)},
        {"textureName", wo->textureName},
        {"childObjects", json::array()}
    };
    // // child objects
    // for (int i = 0; i < wo->childObjects.size(); i++)
    // {
    //     worldObjectJson["childObjects"].push_back(wo->childObjects.at(i)->id);
    // }
    return worldObjectJson;
}
json Save::saveTransformAsJson(graf::Transform* transform)
{
    json jsonData;

    // glm::vec3 verilerini JSON'e dönüştür
    jsonData["position"] = vec3ToJson(transform->m_position);
    jsonData["euler"] = vec3ToJson(transform->m_euler);
    jsonData["scale"] = vec3ToJson(transform->m_scale);

    // // glm::mat4 verilerini JSON'e dönüştür
    // jsonData["worldMatrix"] = mat4ToJson(transform->m_worldMatrix);
    // jsonData["scaleMatrix"] = mat4ToJson(transform->m_scaleMatrix);
    // jsonData["rotationMatrix"] = mat4ToJson(transform->m_rotationMatrix);
    // jsonData["translateMatrix"] = mat4ToJson(transform->m_translateMatrix);

    return jsonData;
}
json Save::savePlayableObjectAsJson(PlayableObject* po)
{
    json jsonData;
    jsonData["worldObject"]= saveWorldObjectAsJson(po); // base class'ı bu sekilde kaydettim.
    jsonData["camera"] = saveCameraAsJson(po->m_camera);
    jsonData["movementSpeed"] = po->movementSpeed;
    jsonData["cameraSpeed"] = po->cameraSpeed;
    return jsonData;
}

json Save::saveCameraAsJson(graf::Camera* camera)
{
    json jsonData;
    // glm::vec3 verilerini JSON'e dönüştür
    jsonData["transform"] = saveTransformAsJson(camera->m_transform);
    jsonData["fov"] = camera->getFovInDeggree();
    jsonData["aspect"] = camera->m_aspect;
    jsonData["near"] = camera->m_near;
    jsonData["far"] = camera->m_far;
    // jsonData["projectionMatrix"] = mat4ToJson(camera->m_projectionMatrix);
    return jsonData;
}


Scene* Save::readFromFileAsJson()
{
    auto manager = getInstance();
    std::string fileName = manager->m_saveFileName;
    std::ifstream ifstream("./save_files/"+fileName);
    json jsonData;
    ifstream >> jsonData;
    ifstream.close();
    Scene* scene = readSceneFromJson(jsonData);
    return scene;
}
Scene* Save::readSceneFromJson(json jsonDataScene)
{
    // idcounter'ın id'sini okuduklarının arasından max yap.
    auto manager = getInstance();
    Scene* scene = new Scene();
    IdCounter::reset();
    scene->reset();

    
    int activePlayableObjectId = jsonDataScene["activePlayableObject"];
    int topCameraId = jsonDataScene["topCamera"];
    int activeObjectId = jsonDataScene["activeObject"];
    int idMax = -1;
    for (int i = 0; i < jsonDataScene["playableObjects"].size(); i++)
    {
        PlayableObject* po = readPlayableObjectFromJson(jsonDataScene["playableObjects"][i]);
        idMax = idMax <= po->id ? po->id:idMax ; 
        scene->addPlayableObject(po);
        if(po->getId() == activePlayableObjectId)
        {
            scene->setActivePlayableObject(po);
        }
        if(po->getId() == topCameraId)
        {
            scene->setTopCamera(po);
        }
    }
    for (int i = 0; i < jsonDataScene["objects"].size(); i++)
    {
        WorldObject* wo= readWorldObjectFromJson(jsonDataScene["objects"][i]);
        idMax = idMax <= wo->id ? wo->id:idMax ; 
        scene->addObject(wo);
        if(wo->getId() == activeObjectId)
        {
            scene->setActiveObject(wo);
        }
    }
    IdCounter::setId(idMax);
   
    manager->scene =scene; 
    return scene;
}
WorldObject* Save::readWorldObjectFromJson(json jsonDataWO)
{

    int id = jsonDataWO["id"];
    graf::Transform* transform = readTransformFromJson(jsonDataWO["transform"]);
    graf::ShapeTypes shapeType = static_cast<graf::ShapeTypes>(jsonDataWO["shapeType"]);
    std::string textureName = jsonDataWO["textureName"];

    // unsigned int* idArr =nullptr;
    // int childObjectSize=0;
    // if (jsonDataWO.contains("childObjects"))
    // {
    //     const json& childObjectsJson = jsonDataWO["childObjects"];
    //     childObjectSize= childObjectsJson.size();
    //     idArr = new unsigned int[childObjectSize];
    //     int i=0;
    //     for (const auto& childId : childObjectsJson) {
    //         idArr[i] = childId ;
    //         i++;
    //     }
    // }
    
    // auto wofs= new ObjectForSave<WorldObject*>();
    // wofs->arr=idArr;
    // wofs->arrSize=childObjectSize;
    // wofs->obj=new WorldObject(id,textureName,shapeType);
    // wofs->obj->setTransform(transform);
    // return wofs;
    WorldObject * wo = new WorldObject(id,textureName,shapeType);
    wo->setTransform(transform);
    
    return wo;
}
graf::Transform* Save::readTransformFromJson(json jsonDataTransform)
{
    glm::vec3 position = jsonToVec3(jsonDataTransform["position"]);
    glm::vec3 euler = jsonToVec3(jsonDataTransform["euler"]);
    glm::vec3 scale = jsonToVec3(jsonDataTransform["scale"]);
    // glm::mat4 worldMatrix = jsonToMat4(jsonDataTransform["worldMatrix"]) ;
    // glm::mat4 rotationMatrix = jsonToMat4(jsonDataTransform["rotationMatrix"]);
    // glm::mat4 translateMatrix = jsonToMat4(jsonDataTransform["translateMatrix"]);
    // glm::mat4 scaleMatrix = jsonToMat4(jsonDataTransform["scaleMatrix"]) ;
    graf::Transform* transform= new graf::Transform(position,euler,scale);
    return transform;
}
graf::Camera* Save::readCameraFromJson(json jsonDataCamera)
{
    graf::Transform* transform = readTransformFromJson(jsonDataCamera["transform"]);
    float fov =jsonDataCamera["fov"] ;
    float aspect =jsonDataCamera["aspect"] ;
    float near =jsonDataCamera["near"];
    float far =jsonDataCamera["far"] ;
    // glm::mat4 projectionMatrix = jsonToMat4(jsonDataCamera["projectionMatrix"]) ;
    graf::Camera* camera = new graf::Camera(fov,aspect,near,far,transform); 
    return camera;
}
PlayableObject* Save::readPlayableObjectFromJson(json jsonDataPO)
{
    WorldObject*  wo = readWorldObjectFromJson(jsonDataPO["worldObject"]);
    graf::Camera* camera = readCameraFromJson(jsonDataPO["camera"]);
    double movementSpeed= jsonDataPO["movementSpeed"];
    double cameraSpeed= jsonDataPO["cameraSpeed"];

    PlayableObject* po = new PlayableObject(wo);
    po->setCamera(camera);
    po->setMovementSpeed(movementSpeed);
    po->setCameraSpeed(cameraSpeed);
    return po;
}

json Save::vec3ToJson(const glm::vec3& vec) {
    return json{{"x", vec.x}, {"y", vec.y}, {"z", vec.z}};
}

// glm::mat4'ü JSON'e dönüştürmek için bir yardımcı fonksiyon
json Save::mat4ToJson(const glm::mat4& mat) {
    json matrixJson;
    const float* matrixValues = glm::value_ptr(mat);
    for (int i = 0; i < 16; ++i) {
        matrixJson.push_back(matrixValues[i]);
    }
    return matrixJson;
}
glm::vec3 Save::jsonToVec3(const json& j) {
    return glm::vec3(
        j.at("x").get<float>(), 
        j.at("y").get<float>(), 
        j.at("z").get<float>()
    );
}

glm::mat4 Save::jsonToMat4(const json& j) {
    glm::mat4 mat(1.0f);  // Başlangıç olarak birim matris (identity matrix)

    for (int i = 0; i < 16; ++i) {
        mat[i / 4][i % 4] = j.at(i).get<float>(); // JSON dizisindeki veriyi matris elemanlarına aktar
    }

    return mat;
}






























// void Save::saveToFileAsBinary()
// {
//     auto instance = getInstance();
//     std::ofstream ostreamFile(instance->m_saveFileName , std::ios::binary);

//     Save::saveSceneAsBinary(instance->scene,&ostreamFile);

//     ostreamFile.close();
// }
// void Save::saveSceneAsBinary(Scene* scene,std::ofstream* ofstreamFile){
//     size_t totalWorldObjectSize = scene->m_objects.size();
//     ofstreamFile->write(reinterpret_cast<const char*>(&totalWorldObjectSize), sizeof(totalWorldObjectSize));

//     for (int i = 0; i < scene->m_objects.size(); i++)
//     {
//         Save::saveWorldObjectAsBinary(scene->m_objects.at(i),ofstreamFile);
//     }

// }
// void Save::saveWorldObjectAsBinary(WorldObject* wo,std::ofstream* ofstreamFile){
//     //id 
//     ofstreamFile->write(reinterpret_cast<const char*>(&wo->id), sizeof(wo->id));

//     // transform
//     Save::saveTransformAsBinary(wo->transform,ofstreamFile);
    
//     // shape type enum
//     uint8_t shapeTypeCastedInt = static_cast<uint8_t>(wo->shapeType);
//     ofstreamFile->write(reinterpret_cast<const char*>(&shapeTypeCastedInt), sizeof(shapeTypeCastedInt));

//     // texture name
//     size_t textureNameSize = wo->textureName.size();
//     ofstreamFile->write(reinterpret_cast<const char*>(&textureNameSize), sizeof(textureNameSize));
//     ofstreamFile->write(wo->textureName.data(), textureNameSize);
    
//     // child objects
//     size_t childObjectSize = wo->childObjects.size();
//     ofstreamFile->write(reinterpret_cast<const char*>(&childObjectSize), sizeof(childObjectSize));
//     for (int i = 0; i < childObjectSize; i++)
//     {
//         ofstreamFile->write(reinterpret_cast<const char*>(wo->childObjects.at(i)->id ), sizeof(unsigned int));
//     }

// }
// void Save::saveTransformAsBinary(graf::Transform* transform,std::ofstream* ofstreamFile)
// {
//     // position
//     ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_position), sizeof(transform->m_position));
//     // euler
//     ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_euler), sizeof(transform->m_euler));
//     // world matrix
//     ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_worldMatrix), sizeof(transform->m_worldMatrix));
//     // rotation matrix
//     ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_rotationMatrix), sizeof(transform->m_rotationMatrix));
//     // translate matrix
//     ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_translateMatrix), sizeof(transform->m_translateMatrix));
        
// }


// Scene* Save::readFromFileAsBinary()
// {
//     auto instance = getInstance();
//     std::ifstream ifstreamFile(instance->m_saveFileName , std::ios::binary);
//     auto shaderProgram = graf::ShaderManager::getShaderProgram("TextureShader");
//     Scene* scene = instance->readSceneFromBinary(ifstreamFile);

//     ifstreamFile.close();
//     instance->scene = scene;
//     return scene;
// }



// Scene* Save::readSceneFromBinary(std::ifstream& ifstreamFile)
// {
//     size_t totalWorldObjectSize;
//     ifstreamFile.read(reinterpret_cast<char*>(&totalWorldObjectSize), sizeof(totalWorldObjectSize));

//     std::vector<ObjectForSave<WorldObject*>*> woVector;
//     for (size_t i = 0; i < totalWorldObjectSize; i++)
//     {
//         ObjectForSave<WorldObject*>* worldObjectForSave = this->readWorldObjectFromBinary(ifstreamFile);
//         woVector.push_back(worldObjectForSave);
//     }

    
//     Scene* scene = new Scene();


//     // Bütün objeleri oluşturduktan sonra. Objelerin cocuk objelerini atadım.
//     //  
//     int vectorSize = woVector.size();
//     for (size_t i = 0; i < vectorSize; i++)
//     {
//         int arrSize = woVector.at(i)->arrSize;  
//         for (size_t j = 0; j <arrSize ; j++)
//         {
//             WorldObject* childObject = IdCounter::getWorldObjectById(woVector.at(i)->arr[j]);
//             woVector.at(i)->obj->addChildObject(childObject);
//         }
//         scene->addObject(woVector.at(i)->obj);
//     }
//     // Cocuk objeleri atamak için oluşturdugum sınıfta heap alanında oluşturdugu başka
//     // nesneleri serbest bıraktım
    
//     for (size_t i = 0; i < vectorSize; i++)
//     {
//         delete[] woVector.at(i)->arr;
//         delete woVector.at(i);
//     }
    
    
//     return scene;
// }

// ObjectForSave<WorldObject*>* Save::readWorldObjectFromBinary(std::ifstream& ifstreamFile)
// {
//     unsigned int id;
//     graf::Transform* transform;
//     uint8_t shapeTypeCastedInt; // graf::ShapeTypes
//     graf::ShapeTypes shapeType;
//     std::string textureName;

//     // id
//     ifstreamFile.read(reinterpret_cast<char*>(&id), sizeof(id));

//     // transform
//     transform = this->readTransformFromBinary(ifstreamFile);

//     // shape type
//     ifstreamFile.read(reinterpret_cast<char*>(&shapeTypeCastedInt), sizeof(shapeTypeCastedInt));
//     shapeType= static_cast<graf::ShapeTypes>(shapeTypeCastedInt);
//     // texture name
//     size_t textureNameSize;
//     ifstreamFile.read(reinterpret_cast<char*>(&textureNameSize), sizeof(textureNameSize));
//     char geciciokuma[textureNameSize+1]; // \0 karakteri için +1 koydum
//     ifstreamFile.read(reinterpret_cast<char*>(&geciciokuma), textureNameSize);
//     geciciokuma[textureNameSize] ='\0'; // son karaktere end string koydum
//     textureName = geciciokuma;
//     // childobjects 
//     size_t childObjectSize;
//     ifstreamFile.read(reinterpret_cast<char*>(&childObjectSize), sizeof(childObjectSize));
//     unsigned int* idArr = nullptr;
//     if(childObjectSize>0)
//     {
//         idArr = new unsigned int[childObjectSize];
//         ifstreamFile.read(reinterpret_cast<char*>(idArr), childObjectSize * sizeof(unsigned int));
//     }

//     auto wofs= new ObjectForSave<WorldObject*>();
//     wofs->arr=idArr;
//     wofs->arrSize=childObjectSize;
//     wofs->obj=new WorldObject(id,textureName,shapeType);
//     wofs->obj->setTransform(transform);
//     return wofs;
// }
// graf::Transform* Save::readTransformFromBinary(std::ifstream& ifstreamFile)
// {
//     glm::vec3 position;
//     glm::vec3 euler;
//     glm::mat4 worldMatrix;
//     glm::mat4 rotationMatrix;
//     glm::mat4 translateMatrix;

//     // position
//     ifstreamFile.read(reinterpret_cast<char*>(&position), sizeof(position));

//     // euler
//     ifstreamFile.read(reinterpret_cast<char*>(&euler), sizeof(euler));

//     // world matrix
//     ifstreamFile.read(reinterpret_cast<char*>(&worldMatrix), sizeof(worldMatrix));
    
//     // rotation matrix
//     ifstreamFile.read(reinterpret_cast<char*>(&rotationMatrix), sizeof(rotationMatrix));
    
//     // translate matrix
//     ifstreamFile.read(reinterpret_cast<char*>(&translateMatrix), sizeof(translateMatrix));

//     graf::Transform* transform = new graf::Transform(position,euler,glm::vec3(1.0f),worldMatrix,rotationMatrix,translateMatrix,glm::mat4(1.0f));
//     return transform;
// }
