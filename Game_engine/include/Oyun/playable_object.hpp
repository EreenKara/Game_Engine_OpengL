#ifndef playable_object_hpp
#define playable_object_hpp
#include "worldobject.hpp"
#include "camera.hpp"
#include "mouse.hpp"

class PlayableObject:WorldObject
{
private:
    Mouse* m_mouse; 
    graf::Camera* m_camera;
    double movementSpeed;
    double cameraSpeed;
public:
    graf::Camera* getCamera(); 
    void keyboardFunction(int key,int scancode,int action);
    void mouseFunction(double xpos,double ypos);
    PlayableObject();
    ~PlayableObject();
};



#endif