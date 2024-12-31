#ifndef settings_hpp
#define settings_hpp


class Settings
{
private:
    unsigned int m_screenWidth;
    unsigned int m_screenHeight;
    unsigned int m_screenTopCameraWidth;
    unsigned int m_screenTopCameraHeight;
    static Settings* m_Ins;
    Settings();
    static Settings* getInstance();

public:
    static unsigned int getScreenWidth();
    static unsigned int getScreenHeight();
    static unsigned int getScreenTopCameraWidth();
    static unsigned int getScreenTopCameraHeight();
    static void setScreenWidth(unsigned int width);
    static void setScreenHeight(unsigned int height);
    static void setScreenTopCameraWidth(unsigned int width);
    static void setScreenTopCameraHeight(unsigned int height);
    ~Settings();


};

#endif
