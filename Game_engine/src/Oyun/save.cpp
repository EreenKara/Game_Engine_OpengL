#include "save.hpp"
#include <fstream>
#include "idcounter.hpp"
#include <vector>
#include "nlohmann/json.hpp"
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

    saveSceneAsJson(manager->scene,&ofstreamFile);
    ofstreamFile.close();

}

void Save::saveSceneAsJson(Scene* scene,std::ofstream* ofstreamFile)
{
    json j = json::parse("");
    *ofstreamFile << j;

    for (int i = 0; i < scene->m_objects.size(); i++)
    {
        Save::saveWorldObjectAsBinary(scene->m_objects.at(i),ofstreamFile);
    }

}
void Save::saveWorldObjectAsJson()
{

}
void Save::saveTransformAsJson()
{

}
Scene* Save::readFromFileAsJson()
{

}
Scene* Save::readSceneFromJson(std::ifstream& ifstreamFile)
{

}
ObjectForSave<WorldObject*>* Save::readWorldObjectFromJson(std::ifstream& ifstreamFile)
{

}
graf::Transform* Save::readTransformFromJson(std::ifstream& ifstreamFile)
{

}























void Save::saveToFileAsBinary()
{
    auto instance = getInstance();
    std::ofstream ostreamFile(instance->m_saveFileName , std::ios::binary);

    Save::saveSceneAsBinary(instance->scene,&ostreamFile);

    ostreamFile.close();
}
void Save::saveSceneAsBinary(Scene* scene,std::ofstream* ofstreamFile){
    size_t totalWorldObjectSize = scene->m_objects.size();
    ofstreamFile->write(reinterpret_cast<const char*>(&totalWorldObjectSize), sizeof(totalWorldObjectSize));

    for (int i = 0; i < scene->m_objects.size(); i++)
    {
        Save::saveWorldObjectAsBinary(scene->m_objects.at(i),ofstreamFile);
    }

}
void Save::saveWorldObjectAsBinary(WorldObject* wo,std::ofstream* ofstreamFile){
    //id 
    ofstreamFile->write(reinterpret_cast<const char*>(&wo->id), sizeof(wo->id));

    // transform
    Save::saveTransformAsBinary(wo->transform,ofstreamFile);
    
    // shape type enum
    uint8_t shapeTypeCastedInt = static_cast<uint8_t>(wo->shapeType);
    ofstreamFile->write(reinterpret_cast<const char*>(&shapeTypeCastedInt), sizeof(shapeTypeCastedInt));

    // texture name
    size_t textureNameSize = wo->textureName.size();
    ofstreamFile->write(reinterpret_cast<const char*>(&textureNameSize), sizeof(textureNameSize));
    ofstreamFile->write(wo->textureName.data(), textureNameSize);
    
    // child objects
    size_t childObjectSize = wo->childObjects.size();
    ofstreamFile->write(reinterpret_cast<const char*>(&childObjectSize), sizeof(childObjectSize));
    for (int i = 0; i < childObjectSize; i++)
    {
        ofstreamFile->write(reinterpret_cast<const char*>(wo->childObjects.at(i)->id ), sizeof(unsigned int));
    }

}
void Save::saveTransformAsBinary(graf::Transform* transform,std::ofstream* ofstreamFile)
{
    // position
    ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_position), sizeof(transform->m_position));
    // euler
    ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_euler), sizeof(transform->m_euler));
    // world matrix
    ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_worldMatrix), sizeof(transform->m_worldMatrix));
    // rotation matrix
    ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_rotationMatrix), sizeof(transform->m_rotationMatrix));
    // translate matrix
    ofstreamFile->write(reinterpret_cast<const char*>(&transform->m_translateMatrix), sizeof(transform->m_translateMatrix));
        
}


Scene* Save::readFromFileAsBinary()
{
    auto instance = getInstance();
    std::ifstream ifstreamFile(instance->m_saveFileName , std::ios::binary);
    auto shaderProgram = graf::ShaderManager::getShaderProgram("TextureShader");
    Scene* scene = instance->readSceneFromBinary(ifstreamFile);

    ifstreamFile.close();
    instance->scene = scene;
    return scene;
}
bool Save::isThereReadableSaveFile()
{
    auto instance = getInstance();
    std::ifstream file(instance->m_saveFileName);
    bool readable = file ? true:false;
    file.close();
    return readable;
}


Scene* Save::readSceneFromBinary(std::ifstream& ifstreamFile)
{
    size_t totalWorldObjectSize;
    ifstreamFile.read(reinterpret_cast<char*>(&totalWorldObjectSize), sizeof(totalWorldObjectSize));

    std::vector<ObjectForSave<WorldObject*>*> woVector;
    for (size_t i = 0; i < totalWorldObjectSize; i++)
    {
        ObjectForSave<WorldObject*>* worldObjectForSave = this->readWorldObjectFromBinary(ifstreamFile);
        woVector.push_back(worldObjectForSave);
    }

    
    Scene* scene = new Scene();


    // Bütün objeleri oluşturduktan sonra. Objelerin cocuk objelerini atadım.
    //  
    int vectorSize = woVector.size();
    for (size_t i = 0; i < vectorSize; i++)
    {
        int arrSize = woVector.at(i)->arrSize;  
        for (size_t j = 0; j <arrSize ; j++)
        {
            WorldObject* childObject = IdCounter::getWorldObjectById(woVector.at(i)->arr[j]);
            woVector.at(i)->obj->addChildObject(childObject);
        }
        scene->addObject(woVector.at(i)->obj);
    }
    // Cocuk objeleri atamak için oluşturdugum sınıfta heap alanında oluşturdugu başka
    // nesneleri serbest bıraktım
    
    for (size_t i = 0; i < vectorSize; i++)
    {
        delete[] woVector.at(i)->arr;
        delete woVector.at(i);
    }
    
    
    return scene;
}

ObjectForSave<WorldObject*>* Save::readWorldObjectFromBinary(std::ifstream& ifstreamFile)
{
    unsigned int id;
    graf::Transform* transform;
    uint8_t shapeTypeCastedInt; // graf::ShapeTypes
    graf::ShapeTypes shapeType;
    std::string textureName;

    // id
    ifstreamFile.read(reinterpret_cast<char*>(&id), sizeof(id));

    // transform
    transform = this->readTransformFromBinary(ifstreamFile);

    // shape type
    ifstreamFile.read(reinterpret_cast<char*>(&shapeTypeCastedInt), sizeof(shapeTypeCastedInt));
    shapeType= static_cast<graf::ShapeTypes>(shapeTypeCastedInt);
    // texture name
    size_t textureNameSize;
    ifstreamFile.read(reinterpret_cast<char*>(&textureNameSize), sizeof(textureNameSize));
    char geciciokuma[textureNameSize+1]; // \0 karakteri için +1 koydum
    ifstreamFile.read(reinterpret_cast<char*>(&geciciokuma), textureNameSize);
    geciciokuma[textureNameSize] ='\0'; // son karaktere end string koydum
    textureName = geciciokuma;
    // childobjects 
    size_t childObjectSize;
    ifstreamFile.read(reinterpret_cast<char*>(&childObjectSize), sizeof(childObjectSize));
    unsigned int* idArr = nullptr;
    if(childObjectSize>0)
    {
        idArr = new unsigned int[childObjectSize];
        ifstreamFile.read(reinterpret_cast<char*>(idArr), childObjectSize * sizeof(unsigned int));
    }

    auto wofs= new ObjectForSave<WorldObject*>();
    wofs->arr=idArr;
    wofs->arrSize=childObjectSize;
    wofs->obj=new WorldObject(id,textureName,shapeType);
    wofs->obj->setTransform(transform);
    return wofs;
}
graf::Transform* Save::readTransformFromBinary(std::ifstream& ifstreamFile)
{
    glm::vec3 position;
    glm::vec3 euler;
    glm::mat4 worldMatrix;
    glm::mat4 rotationMatrix;
    glm::mat4 translateMatrix;

    // position
    ifstreamFile.read(reinterpret_cast<char*>(&position), sizeof(position));

    // euler
    ifstreamFile.read(reinterpret_cast<char*>(&euler), sizeof(euler));

    // world matrix
    ifstreamFile.read(reinterpret_cast<char*>(&worldMatrix), sizeof(worldMatrix));
    
    // rotation matrix
    ifstreamFile.read(reinterpret_cast<char*>(&rotationMatrix), sizeof(rotationMatrix));
    
    // translate matrix
    ifstreamFile.read(reinterpret_cast<char*>(&translateMatrix), sizeof(translateMatrix));

    graf::Transform* transform = new graf::Transform(position,euler,worldMatrix,rotationMatrix,translateMatrix);
    return transform;
}
