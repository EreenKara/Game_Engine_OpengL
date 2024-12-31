#include "cursor.hpp"

Cursor::Cursor(WorldObject* wo) : WorldObject(*wo)
{
    this->transform->rotateGobalX(180);
}
glm::vec3 Cursor::calculateCursorPosition(WorldObject* wo)
{
    auto transform = wo->getTranform();
    auto position = transform->getPosition(); // degisiklik yapamazsın.
    auto scale = transform->getScale();
    float cisimBoyut = 0.5;
    float cursorBoyut = 0.5;
    // float asagiYukariPayi = 0.5;
    float cisim = (position.y + (cisimBoyut* scale.y));
    float yPosition =  cisim + cursorBoyut + this->asagiYukariCursorPay -  this->asagiYukariCursor;  
    position.y = yPosition;
    asagiYukariCursor += artis;
    if(asagiYukariCursor>= asagiYukariCursorPay)
    {
        artis=-0.001f;
    } 
    else if(asagiYukariCursor<=0){
        artis=0.001f;
    }   
    
    this->transform->setPosition(position);
    return position;
}