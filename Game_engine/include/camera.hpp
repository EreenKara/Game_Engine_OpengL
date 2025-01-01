#ifndef camera_hpp
#define camera_hpp

#include "glm/glm.hpp"
#include "transform.hpp"
class Save;

namespace graf
{
    class Transform;
    class Camera
    {
    private:
        friend class ::Save;
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
        float getFov() const;
        float getAspect() const;
        float getNear() const;
        float getFar() const;
        float getFovInDeggree();
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();
        Transform* getTransform();
        void setTransform(Transform* transform);
        void turnLR(float angle);
        void turnUD(float angle);
        Camera(float fovDegree=90.0f,float aspect=1.78f,float near=1.0f,float far=100.0f,Transform* transform = new Transform());
        ~Camera();
    };
} // namespace graf





#endif