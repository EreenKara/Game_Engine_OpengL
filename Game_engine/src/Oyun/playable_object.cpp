#include "playable_object.hpp"
#include "glwindow.hpp"

PlayableObject::PlayableObject(const WorldObject* wo):
                               WorldObject(*wo){

    movementSpeed = 0.1f;
    cameraSpeed= 0.1f;
    m_mouse=new Mouse();
    m_camera =new graf::Camera();
    this->shapeType=graf::ShapeTypes::Frustum;
    // this->shaderProgramName="KenarShader";
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
            this->m_camera->getTransform()->moveForward();
            this->transform->setPosition(this->m_camera->getTransform()->getPosition());
        }
        else if(key==GLFW_KEY_S)
        {
            this->m_camera->getTransform()->moveBackward();
            this->transform->setPosition(this->m_camera->getTransform()->getPosition());
        }
        else if(key == GLFW_KEY_A)
        {
            this->m_camera->getTransform()->moveLeft();
            this->transform->setPosition(this->m_camera->getTransform()->getPosition());
        } 
        else if(key == GLFW_KEY_D)
        {
            this->m_camera->getTransform()->moveRight();
            this->transform->setPosition(this->m_camera->getTransform()->getPosition());
        }
        else if(key == GLFW_KEY_LEFT_CONTROL)
        {
            this->m_camera->getTransform()->moveUp();
            this->transform->setPosition(this->m_camera->getTransform()->getPosition());
        }
        else if(key == GLFW_KEY_LEFT_SHIFT)
        {
            this->m_camera->getTransform()->moveDown();
            this->transform->setPosition(this->m_camera->getTransform()->getPosition());
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
        this->turnLR(dx);
        this->turnUD(dy);
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
    delete m_camera;
    m_camera= camera;
}

void PlayableObject::turnLR(float angle)
{
    this->m_camera->turnLR(angle);
    auto euler =this->m_camera->getTransform()->getEuler();
    this->transform->setEuler(euler);

}
// TURN UP OR DOWN
void PlayableObject::turnUD(float angle)// TURN LEFT OR RIGHTturnUD(float angle)
{
    this->m_camera->turnUD(angle);
    auto cameraEuler = this->m_camera->getTransform()->getEuler();
    cameraEuler.x = cameraEuler.x+90;
    this->transform->setEuler(cameraEuler);
}


PlayableObject::~PlayableObject(){
    delete m_mouse;
    delete m_camera;
}