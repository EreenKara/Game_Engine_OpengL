#include "settings.hpp"

Settings* Settings::m_Ins = nullptr;

Settings::Settings()
{
    m_screenWidth = 1920;
    m_screenHeight = 1080;
    m_screenTopCameraWidth= 500;
    m_screenTopCameraHeight =400;
}
Settings* Settings::getInstance()
{
    if(m_Ins==nullptr)
    {
        m_Ins = new Settings();
    }

    return m_Ins;
}
unsigned int Settings::getScreenWidth()
{
    auto instance = getInstance();
    return instance->m_screenWidth;
}
unsigned int Settings::getScreenHeight()
{
    auto instance = getInstance();
    return instance->m_screenHeight;
}
unsigned int Settings::getScreenTopCameraWidth()
{
    auto instance = getInstance();
    return instance->m_screenTopCameraWidth;
}
unsigned int Settings::getScreenTopCameraHeight()
{
    auto instance = getInstance();
    return instance->m_screenTopCameraHeight;
}
void Settings::setScreenWidth(unsigned int width)
{
    auto instance = getInstance();
    instance->m_screenWidth= width;
}
void Settings::setScreenHeight(unsigned int height)
{
    auto instance = getInstance();
    instance->m_screenHeight= height;
}
void Settings::setScreenTopCameraWidth(unsigned int width)
{
    auto instance = getInstance();
    instance->m_screenTopCameraWidth= width;
}
void Settings::setScreenTopCameraHeight(unsigned int height)
{
    auto instance = getInstance();
    instance->m_screenTopCameraHeight= height;
}