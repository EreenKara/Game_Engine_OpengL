#ifndef idcounter_hpp
#define idcounter_hpp
#include <unordered_map>
#include "worldobject.hpp"
// aslında m_id 'yide static yapsam singleton'a gerek kalmayacak
// singleton yapmış bulundum
class IdCounter
{
private:
    friend class Save;
    friend class WorldObject;
    static void setId(unsigned int id);
    static void setWorldObjectForId(unsigned int id, WorldObject* worldObject);
    // id -1 ise hiçbir obje başlatılmamış
    unsigned int m_id;
    static IdCounter* m_Ins;
    IdCounter();
    static void reset();
    static IdCounter* getInstance();
    static void deleteInstance();
    ~IdCounter();
    std::unordered_map<unsigned int, WorldObject*> idToWorldObject;
public:
    // en son verilen id
    static unsigned int getCurrentID();
    // sıradaki yaratılan obejeye verilecek id
    static unsigned int nextID(WorldObject* wo);
    // verieln id'ye karşılık herheangi bir obje var mı?
    static bool idInUse(unsigned int id);
    static WorldObject* getWorldObjectById(unsigned int id);
};




#endif