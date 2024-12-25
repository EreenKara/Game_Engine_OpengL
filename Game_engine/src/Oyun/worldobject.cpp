#include "worldobject.hpp"
#include <shapecreator.hpp>
#include <vertexarrayobject.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <texturemanager.hpp>
#include "shaderprogram.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include "idcounter.hpp"


WorldObject::WorldObject(unsigned int id,
                         std::string textureName,
                         graf::ShapeTypes shapeType
                         )   
{
    if(id!=-1)
    {
        if(!IdCounter::idInUse(id))
        {
            this->id =id;
            IdCounter::setWorldObjectForId(id,this);
        }
        else
        {
            throw std::invalid_argument("Hatali bir id atanmaya kalkişildi");
        }
    }
    else{
        this->id = IdCounter::nextID(this);
    }
            
    this->transform = new graf::Transform();
    this->textureName=textureName;
    this->shapeType = shapeType;
}
graf::Transform* WorldObject::getTranform() const
{
    return this->transform;
}
void WorldObject::setTransform(graf::Transform* transform)
{
    this->transform = transform;
}

void WorldObject::changeTexture(const std::string& textureName){
    this->textureName=textureName;
}
void WorldObject::changeShape(const graf::ShapeTypes shapeType){
    this->shapeType= shapeType;
}
const graf::ShapeTypes WorldObject::getShapeType() const{
    return this->shapeType;
}
const std::string WorldObject::getTextureName() const{
    return this->textureName;
}
void WorldObject::addChildObject(WorldObject* worldObject)
{
    this->childObjects.push_back(worldObject);
}
void WorldObject::removeChildObject(WorldObject* worldObject)
{
    
    if(std::find(this->childObjects.begin(), this->childObjects.end(), worldObject) != this->childObjects.end())
    {
        this->childObjects.erase(
        std::remove(this->childObjects.begin(), this->childObjects.end(), worldObject),
        this->childObjects.end());
    }

}
std::vector<WorldObject*>* WorldObject::getChildObjects()
{
    return &(this->childObjects);
}

unsigned int WorldObject::getId()
{
    return this->id;
}


WorldObject::~WorldObject(){
    delete this->transform;
}