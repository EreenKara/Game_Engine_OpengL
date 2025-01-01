#include "camera.hpp"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

namespace graf
{
    
    Camera::Camera(float fovDegree, float aspect, float near, float far,Transform* transform)
    {
        m_fov=glm::radians(fovDegree);
        m_aspect=aspect;
        m_near=near;
        m_far=far;
        m_transform = transform;
        m_projectionMatrix= glm::perspectiveLH(m_fov,m_aspect,m_near,m_far);
    }

    Camera::~Camera()
    {
        delete m_transform;
    }
    Transform* Camera::getTransform(){
        return m_transform;
    }
    void Camera::setTransform(Transform* transform){
        delete m_transform;
        m_transform = transform;
    }
    glm::mat4 Camera::getViewMatrix(){
        auto mtxInvTranslate = glm::translate(glm::mat4(1),-m_transform->getPosition());
        auto mtxInvRotation = glm::inverse(m_transform->getRotationMatrix());
        // burada ilk önce merkeze çektik kamerayı ondan sonra rotasyon işlemi uyguladık.
        auto viewMatrix = mtxInvRotation*mtxInvTranslate;
        return viewMatrix;
    }
    glm::mat4 Camera::getProjectionMatrix()
    {
        return m_projectionMatrix;
    }
    // TURN LEFT OR RIGHT
    void Camera::turnLR(float angle)
    {
        auto euler = m_transform->getEuler();
        euler.y+=angle;
        m_transform->setEuler(euler);

    }
    float Camera::getFovInDeggree()
    {
        return glm::degrees(m_fov);
    }
    // TURN UP OR DOWN
    void Camera::turnUD(float angle)// TURN LEFT OR RIGHTturnUD(float angle)
    {
        auto euler = m_transform->getEuler();
        euler.x+=angle;
        m_transform->setEuler(euler);
    }
    void Camera::setFov(float fovDegree)
    {
        m_fov = glm::radians(fovDegree);
        m_projectionMatrix= glm::perspectiveLH(m_fov,m_aspect,m_near,m_far);
    }
    void Camera::setAspect(float aspect)
    {
        m_aspect = aspect;
        m_projectionMatrix= glm::perspectiveLH(m_fov,m_aspect,m_near,m_far);
    }
    void Camera::setNear(float near)
    {
        m_near=near;
        m_projectionMatrix= glm::perspectiveLH(m_fov,m_aspect,m_near,m_far);
    }
    void Camera::setFar(float far)
    {
        m_far = far;
        m_projectionMatrix= glm::perspectiveLH(m_fov,m_aspect,m_near,m_far);
    }
} // namespace graf
