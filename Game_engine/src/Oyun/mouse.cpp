#include "mouse.hpp"

double Mouse::getCurrentXpos(){
    return m_currentXpos;
}
double Mouse::getCurrentYpos(){
    return m_currentYpos;
}
double Mouse::getPreviousXpos(){
    return m_previousXpos;
}
double Mouse::getPreviousYpos(){
    return m_previousYpos;
}
void Mouse::setCurrentXpos(double xpos){
    m_previousXpos=m_currentXpos;
    m_currentXpos = xpos;
}
void Mouse::setCurrentYpos(double ypos)
{
    m_previousYpos=m_currentYpos;
    m_currentYpos = ypos;
}
Mouse::Mouse(){
    m_currentXpos      = -1;
    m_previousXpos      = -1;
    m_currentYpos      = -1;
    m_previousYpos      = -1;
}
Mouse::~Mouse(){

}