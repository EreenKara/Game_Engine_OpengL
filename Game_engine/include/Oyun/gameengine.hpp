#ifndef gameengine_hpp
#define gameengine_hpp
#include "glwindow.hpp"
#include "shaderprogram.hpp"
#include "shapecreator.hpp"
#include "glm/glm.hpp"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "Scene.hpp"

class GameEngine
{
private:
    Scene* m_scene;
    graf::GLWindow m_window;
    graf::ShaderProgram* m_shaderProgram;
    void createShaderProgram(const std::string& fileName);
    void preload();
    void preloadTextures();
    void preloadSomeShapes();
public:
    GameEngine();
    void sceneLoad();
    ~GameEngine();
};

#endif