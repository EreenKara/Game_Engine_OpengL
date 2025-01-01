#ifndef save_hpp
#define save_hpp
#include <string>
#include "transform.hpp"
#include "worldobject.hpp"
#include "scene.hpp"
#include "shaderprogram.hpp"
#include "nlohmann/json.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr için
using json = nlohmann::json;

class Scene;
class Camera;
class PlayableObject;
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
    // std::string m_saveFileNameBinary = "./save_files/oyunDosyasi.erensavebin";
    std::string m_saveFileName = "newSave.json";
    static Save* m_Ins;
    Scene* scene;
    static Save* getInstance();
    static void deleteInstance();
    Save();
    ~Save();
    // Scene* readSceneFromBinary(std::ifstream& ifstreamFile);
    // ObjectForSave<WorldObject*>* readWorldObjectFromBinary(std::ifstream& ifstreamFile);
    // graf::Transform* readTransformFromBinary(std::ifstream& ifstreamFile);
    // static void saveWorldObjectAsBinary(WorldObject* wo,std::ofstream* ofstreamFile);
    // static void saveSceneAsBinary(Scene* scene,std::ofstream* ostreamFile);
    // static void saveTransformAsBinary(graf::Transform* transform,std::ofstream* ofstreamFile);


    static json vec3ToJson(const glm::vec3& vec);
    static json vec2ToJson(const glm::vec2& vec);
    static json mat4ToJson(const glm::mat4& mat);
    static glm::vec3 jsonToVec3(const json& j) ;
    static glm::vec2 jsonToVec2(const json& j) ;
    static glm::mat4 jsonToMat4(const json& j);

    static json saveSceneAsJson();
    static json saveWorldObjectAsJson(WorldObject* wo);
    static json saveTransformAsJson(graf::Transform* transform);
    static json saveCameraAsJson(graf::Camera *camera);
    static json savePlayableObjectAsJson(PlayableObject* po);

    static Scene* readSceneFromJson(json jsonData);
    static WorldObject* readWorldObjectFromJson(json jsonData);
    static graf::Transform* readTransformFromJson(json jsonData);
    static graf::Camera* readCameraFromJson(json jsonData);
    static PlayableObject* readPlayableObjectFromJson(json jsonData);
public:


    static void saveToFileAsJson();
    static Scene* readFromFileAsJson();
    static bool isThereReadableSaveFile();
    static void setSceneToSave(Scene* scene);


    // static void saveToFileAsBinary();
    // static Scene* readFromFileAsBinary();
};




#endif