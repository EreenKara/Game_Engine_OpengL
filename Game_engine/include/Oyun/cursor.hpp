#ifndef cursor_hpp
#define cursor_hpp

#include "worldobject.hpp"

class Cursor : public WorldObject
{
private:

    // anlik bir sey kaydetme bu field'ları
    float asagiYukariCursorPay = 0.5;
    float asagiYukariCursor = 0.4;
    float artis=0.001f;
public:
    Cursor(WorldObject* wo);
    glm::vec3 calculateCursorPosition(WorldObject* wo);
    ~Cursor();
};

#endif