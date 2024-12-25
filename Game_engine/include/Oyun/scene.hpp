#ifndef scene_hpp
#define scene_hpp
#include "glwindow.hpp"
#include "shapecreator.hpp"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <vector>
#include "playable_object.hpp"
#include "save.hpp"
#include "shadermanager.hpp"
class WorldObject;
class Scene
{
private:
    friend class Save;
    PlayableObject* playableObject;
    vector<WorldObject*> m_objects;
    std::string shaderProgramName;
    void drawObject(WorldObject* object,PlayableObject* playableObject,graf::ShaderProgram* shaderProgram);
    void deleteWorldObjects();
public:
    Scene();
    void addObject(WorldObject* worldObject);
    void removeObject(WorldObject* worldObject);
    WorldObject* getObject(WorldObject* worldObject);
    WorldObject* getObject(unsigned int id);
    void renderFunction();
    void imguiRenderFunction();
    void keyboardFunction(int key,int scancode,int action);
    void mouseFunction(double x,double y);
    ~Scene();
};



#endif