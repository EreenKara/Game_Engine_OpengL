#ifndef transform_hpp
#define transform_hpp
#include <glm/glm.hpp>


/*
    Transform'daki hareket fonksiyonlarını başka bir sınıfa çekebilirsin.
*/
class Save;
namespace graf{
    
    class Transform
    {
    private:
        friend class ::Save;
        glm::vec3 m_position;
        glm::vec3 m_euler;
        glm::vec3 m_scale;
        glm::mat4 m_worldMatrix;
        glm::mat4 m_rotationMatrix;
        glm::mat4 m_translateMatrix;
        glm::mat4 m_scaleMatrix;
        void update();
    public:
        void moveForward();
        void moveBackward();
        void moveRight();
        void moveLeft();
        void moveUp();
        void moveDown();
        glm::vec3 getLook();
        glm::vec3 getUp();
        glm::vec3 getRight();
        glm::vec3 getPosition();
        glm::vec3 getEuler();
        glm::mat4 getWorldMatrix();
        glm::mat4 getRotationMatrix();
        void setScale(const glm::vec3& scale);
        void rotateLocalX(float angle);
        void rotateLocalY(float angle);
        void rotateLocalZ(float angle);
        void rotateGobalX(float angle);
        void rotateGobalY(float angle);
        void rotateGobalZ(float angle);
        void setPosition(glm::vec3 position);
        void setPosition(float x, float y, float z);
        void setEuler(const glm::vec3& euler);
        // glm::vec3 setEuler(float x, float y, float z);
        // eğerki scale'i verip scalematrix vermezse felan problem olabilir.
        Transform(glm::vec3 position=glm::vec3(0.0f),
                  glm::vec3 euler = glm::vec3(0.0f),
                  glm::vec3 scale = glm::vec3(1.0f),
                  glm::mat4 worldMatrix = glm::mat4(1.0f),
                  glm::mat4 rotationMatrix = glm::mat4(1.0f),
                  glm::mat4 translateMatrix = glm::mat4(1.0f),
                  glm::mat4 scaleMatrix = glm::mat4(1.0f)
        );
        ~Transform();
    };
}



#endif