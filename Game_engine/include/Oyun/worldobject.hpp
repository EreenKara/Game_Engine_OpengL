#ifndef WorldObject_hpp
#define WorldObject_hpp
#include <glm/glm.hpp>
#include "shapecreator.hpp"
#include "transform.hpp"
#include <vector>
#include "shaderprogram.hpp"
#include<glad/glad.h>

class WorldObject // Hocanın model dediği sınıfın bendeki karşılığı
{
protected:
    friend class Save;
    void setTransform(graf::Transform* transform);
    unsigned int id;
    graf::Transform* transform;
    graf::ShapeTypes shapeType;
    std::string textureName;
    std::vector<WorldObject*> childObjects;
    std::string shaderProgramName;
    glm::vec2   textureRepeat;
    unsigned int fillType;
public:
    WorldObject(unsigned int id=-1,
                std::string textureName="container",
                graf::ShapeTypes shapeType=graf::ShapeTypes::Cube,
                std::string shaderProgramName= "TextureShader",
                unsigned int fillType= GL_FILL,
                glm::vec2 textureRepeat = glm::vec2(1.0f));
    std::vector<WorldObject*>* getChildObjects();
    void addChildObject(WorldObject* worldObject);
    void removeChildObject(WorldObject* worldObject);
    void changeTexture(const std::string& textureName);
    void changeShape(const graf::ShapeTypes shapeType);
    const graf::ShapeTypes getShapeType() const; 
    graf::ShaderProgram* getShaderProgram();
    glm::vec2& getTextureRepeat();
    unsigned int getFillType()const;
    const std::string getTextureName() const; 
    graf::Transform* getTranform() const;
    void save(std::ofstream& ostreamFile);
    unsigned int getId();
    ~WorldObject();
};




#endif