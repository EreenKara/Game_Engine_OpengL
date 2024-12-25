#ifndef WorldObject_hpp
#define WorldObject_hpp
#include <glm/glm.hpp>
#include "shapecreator.hpp"
#include "transform.hpp"
#include <vector>

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
public:
    WorldObject(unsigned int id=-1,
                std::string textureName="container",
                graf::ShapeTypes shapeType=graf::ShapeTypes::Cube);
    std::vector<WorldObject*>* getChildObjects();
    void addChildObject(WorldObject* worldObject);
    void removeChildObject(WorldObject* worldObject);
    void changeTexture(const std::string& textureName);
    void changeShape(const graf::ShapeTypes shapeType);
    const graf::ShapeTypes getShapeType() const; 
    const std::string getTextureName() const; 
    graf::Transform* getTranform() const;
    void save(std::ofstream& ostreamFile);
    unsigned int getId();
    ~WorldObject();
};




#endif