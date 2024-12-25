#ifndef mouse_hpp
#define mosue_hpp

class Mouse
{
private:
    double m_currentXpos;
    double m_previousXpos;
    double m_currentYpos;
    double m_previousYpos;
    double m_sensivity;
public:
    double getCurrentXpos();
    void setCurrentXpos(double xpos);
    double getPreviousXpos();
    double getCurrentYpos();
    void setCurrentYpos(double ypos);
    double getPreviousYpos();
    double getSensivity();
    double setSensivity(double sensivity);
    Mouse();
    ~Mouse();
};




#endif