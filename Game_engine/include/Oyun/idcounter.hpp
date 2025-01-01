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
    static void setId(int id);
    static void setWorldObjectForId(int id, WorldObject* worldObject);
    // id -1 ise hiçbir obje başlatılmamış
    int m_id;
    static IdCounter* m_Ins;
    IdCounter();
    static void reset();
    static void deleteInstance();
    ~IdCounter();
    std::unordered_map<int, WorldObject*> idToWorldObject;
public:
    // en son verilen id
    static int getCurrentID();
    // sıradaki yaratılan obejeye verilecek id
    static int nextID(WorldObject* wo);
    // verieln id'ye karşılık herheangi bir obje var mı?
    static bool idInUse(int id);
    static WorldObject* getWorldObjectById(int id);
    static IdCounter* getInstance();

};




#endif