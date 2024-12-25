#ifndef save_hpp
#define save_hpp
#include <string>
#include "transform.hpp"
#include "worldobject.hpp"
#include "scene.hpp"
#include "shaderprogram.hpp"
class Scene;
template <typename T>
struct ObjectForSave
{
    T obj;
    unsigned int* arr;
    size_t arrSize;
};
class Save
{
private:
    std::string m_saveFileNameBinary = "./save_files/oyunDosyasi.erensavebin";
    std::string m_saveFileName = "./save_files/oyunDosyasi.json";
    static Save* m_Ins;
    Scene* scene;
    static Save* getInstance();
    static void deleteInstance();
    Scene* readSceneFromBinary(std::ifstream& ifstreamFile);
    ObjectForSave<WorldObject*>* readWorldObjectFromBinary(std::ifstream& ifstreamFile);
    graf::Transform* readTransformFromBinary(std::ifstream& ifstreamFile);
    Save();
    ~Save();
    static void saveWorldObjectAsBinary(WorldObject* wo,std::ofstream* ofstreamFile);
    static void saveSceneAsBinary(Scene* scene,std::ofstream* ostreamFile);
    static void saveTransformAsBinary(graf::Transform* transform,std::ofstream* ofstreamFile);



    static void saveSceneAsJson(Scene* scene,std::ofstream* ostreamFile);
    static void saveWorldObjectAsJson();
    static void saveTransformAsJson();
    Scene* readSceneFromJson(std::ifstream& ifstreamFile);
    ObjectForSave<WorldObject*>* readWorldObjectFromJson(std::ifstream& ifstreamFile);
    graf::Transform* readTransformFromJson(std::ifstream& ifstreamFile);
public:


    static void saveToFileAsJson();
    static Scene* readFromFileAsJson();
    static bool isThereReadableSaveFile();
    static void setSceneToSave(Scene* scene);


    static void saveToFileAsBinary();
    static Scene* readFromFileAsBinary();
};




#endif