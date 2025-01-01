#include "idcounter.hpp"
IdCounter* IdCounter::m_Ins = nullptr;
IdCounter::IdCounter()
{
    m_id=-1;
}
IdCounter* IdCounter::getInstance()
{
    if(m_Ins==nullptr)
    {
        m_Ins = new IdCounter();
    }

    return m_Ins;
}
void IdCounter::reset()
{
    auto instance = getInstance();
    instance->m_id=-1;
    instance->idToWorldObject.clear();
}

void IdCounter::deleteInstance(){
    if(getInstance() != nullptr)
        delete getInstance();
}
void IdCounter::setId(int id)
{
    auto instance = getInstance();
    instance->m_id=id;
}
void IdCounter::setWorldObjectForId(int id, WorldObject* worldObject)
{
    auto instance = getInstance();
    instance->idToWorldObject[id] = worldObject;
}

int IdCounter::getCurrentID()
{
    auto instance = getInstance();
    return instance->m_id;   
}
int IdCounter::nextID(WorldObject* wo)
{
    auto instance = getInstance();
    if(instance->m_id<10)
    {
        instance->m_id = 10; //reserved for me
    }
    instance->m_id++;
    if (wo) {
        instance->idToWorldObject[instance->m_id] = wo;
    }
    return instance->m_id;
}
bool IdCounter::idInUse(int id)
{
    auto instance = getInstance();
    return instance->idToWorldObject.count(id)>0 ? true:false;
}
WorldObject* IdCounter::getWorldObjectById(int id)
{
    auto instance = getInstance();
    if(!idInUse(id)) return nullptr;
    return instance->idToWorldObject[id];
}
IdCounter::~IdCounter()
{
    deleteInstance();
}