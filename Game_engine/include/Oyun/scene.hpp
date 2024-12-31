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
#include "cursor.hpp"
class WorldObject;
class Scene
{
private:
    friend class Save;
    PlayableObject* activePlayableObject; // ana ekrandaki asıl kamera 
    PlayableObject* topCamera;
    std::vector<PlayableObject*> playableObjects;
    WorldObject* activeObject; // üzerinde değişiklikler yapıalbilecke olan worldobject
    vector<WorldObject*> m_objects;
    Cursor* cursor;


    
    void drawObject(WorldObject* object,PlayableObject* playableObject);
    void deleteWorldObjects();
public:
    Scene();
    void addObject(WorldObject* worldObject);
    void removeObject(WorldObject* worldObject);
    void addPlayableObject(PlayableObject* po);
    void removePlayableObject(PlayableObject* po);
    WorldObject* getObject(WorldObject* worldObject);
    WorldObject* getObject(unsigned int id);
    PlayableObject* getActivePlayableObject();
    PlayableObject* getTopCamera();
    WorldObject* getActiveObject();
    void setActiveObject(WorldObject* wo);
    void setActivePlayableObject(PlayableObject* po);
    void setTopCamera(PlayableObject* po);
    void renderFunction();
    void imguiRenderFunction();
    void keyboardFunction(int key,int scancode,int action);
    void mouseFunction(double x,double y);
    ~Scene();
};



#endif