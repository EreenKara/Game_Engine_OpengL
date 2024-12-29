#ifndef playable_object_hpp
#define playable_object_hpp
#include "worldobject.hpp"
#include "camera.hpp"
#include "mouse.hpp"

class Save;
class PlayableObject:WorldObject
{
private:
    friend Save;
    void setCamera(graf::Camera* camera);
    Mouse* m_mouse; 
    graf::Camera* m_camera;
    double movementSpeed;
    double cameraSpeed;
public:
    graf::Camera* getCamera(); 
    void keyboardFunction(int key,int scancode,int action);
    void mouseFunction(double xpos,double ypos);
    void setMovementSpeed(double movementSpeed);
    void setCameraSpeed(double cameraSpeed);
    PlayableObject(const WorldObject* wo);
    ~PlayableObject();
};



#endif