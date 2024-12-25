#ifndef camera_hpp
#define camera_hpp

#include "glm/glm.hpp"
#include "transform.hpp"

namespace graf
{
    class Transform;
    class Camera
    {
    private:
        Transform* m_transform;
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
        glm::mat4 m_projectionMatrix;
    public:
        void setFov(float fov);
        void setAspect(float aspect);
        void setNear(float near);
        void setFar(float far);
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
        Transform* getTransform();
        void turnLR(float angle);
        void turnUD(float angle);
        Camera(float fovDegree=90.0f,float aspect=1.0f,float near=1.0f,float far=100.0f);
        ~Camera();
    };
} // namespace graf





#endif