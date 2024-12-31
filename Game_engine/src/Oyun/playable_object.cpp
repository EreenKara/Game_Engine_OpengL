#include "playable_object.hpp"
#include "glwindow.hpp"

PlayableObject::PlayableObject(const WorldObject* wo):
                               WorldObject(*wo){

    movementSpeed = 0.1f;
    cameraSpeed= 0.1f;
    m_mouse=new Mouse();
    m_camera =new graf::Camera();
}

graf::Camera* PlayableObject::getCamera()
{
    return m_camera;
} 

void PlayableObject::keyboardFunction(int key,int scancode,int action){
    if(true)
    {
        if(key==GLFW_KEY_W)
        {
            this->transform->moveForward();
            this->m_camera->getTransform()->moveForward();
        }
        else if(key==GLFW_KEY_S)
        {
            this->transform->moveBackward();
            this->m_camera->getTransform()->moveBackward();
        }
        else if(key == GLFW_KEY_A)
        {
            this->transform->moveLeft();
            this->m_camera->getTransform()->moveLeft();
        } 
        else if(key == GLFW_KEY_D)
        {
            this->transform->moveRight();
            this->m_camera->getTransform()->moveRight();
        }
        else if(key == GLFW_KEY_LEFT_CONTROL)
        {
            this->transform->moveUp();
            this->m_camera->getTransform()->moveUp();
        }
        else if(key == GLFW_KEY_LEFT_SHIFT)
        {
            this->transform->moveDown();
            this->m_camera->getTransform()->moveDown();
        }
    }
        
}

void PlayableObject::mouseFunction(double x, double y){
    if(m_mouse->getCurrentXpos()==-1)
    {
        m_mouse->setCurrentXpos(x);
        m_mouse->setCurrentYpos(y);
    }
    else{
        double dx = x - m_mouse->getCurrentXpos() ;
        double dy = y - m_mouse->getCurrentYpos() ;
        dx= dx* this->cameraSpeed;
        dy= dy*this->cameraSpeed;
        m_camera->turnLR(dx);
        m_camera->turnUD(dy);
        m_mouse->setCurrentXpos(x);
        m_mouse->setCurrentYpos(y);    
    }
}
void PlayableObject::setMovementSpeed(double movementSpeed)
{
    this->movementSpeed= movementSpeed;
}
void PlayableObject::setCameraSpeed(double cameraSpeed)
{
    this->cameraSpeed= cameraSpeed;
}
void PlayableObject::setCamera(graf::Camera* camera)
{
    m_camera= camera;
}




PlayableObject::~PlayableObject(){
    delete m_mouse;
    delete m_camera;
}