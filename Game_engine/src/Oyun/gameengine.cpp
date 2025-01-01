#include "gameengine.hpp"
#include "texturemanager.hpp"
#include <functional>
#include "shadermanager.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <string>
#include "settings.hpp"
GameEngine::GameEngine(){
    Settings::setScreenWidth(1920);
    Settings::setScreenHeight(1080);
    Settings::setScreenTopCameraWidth(500);
    Settings::setScreenTopCameraHeight(400);
    m_window.create(Settings::getScreenWidth(),Settings::getScreenHeight());

    this->createShaderProgram("shader_program.json");
    this->preload();
    if (Save::isThereReadableSaveFile())
    {
        // kendi set ediyor içinde
        m_scene = Save::readFromFileAsJson();
    }
    else
    {
        m_scene = new Scene();
        Save::setSceneToSave(m_scene);
    }
    
    auto renderFunction = std::bind(&Scene::renderFunction, m_scene);
    auto imguiRenderFunction = std::bind(&Scene::imguiRenderFunction, m_scene);
    auto keyboardFunction = std::bind(&Scene::keyboardFunction, m_scene,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    auto mouseFunction = std::bind(&Scene::mouseFunction, m_scene,std::placeholders::_1,std::placeholders::_2);
    m_window.setRenderFunction(renderFunction);
    m_window.setImGuiRenderFunction(imguiRenderFunction);
    m_window.setKeyboardFunction(keyboardFunction);
    m_window.setMouseFunction(mouseFunction);
}
void GameEngine::createShaderProgram(const std::string& fileName){

    std::ifstream file("./shaders/"+fileName);
    auto data = nlohmann::json::parse(file);
    
    for (int i = 0; i < data["shaders"].size(); i++)
    {
        graf::ShaderManager::addShaderFromFile(data["shaders"][i]);
    }

}
void GameEngine::preload()
{
    preloadTextures();
    preloadSomeShapes();
}
void GameEngine::preloadTextures()
{
    std::ifstream file("./images/textures.json");
    auto data = nlohmann::json::parse(file);
    auto arr= data["textures"];
    for(auto element:arr)
    {
        std::string textureName = element["textureName"].get<std::string>();
        std::string fileName = element["fileName"].get<std::string>();
        graf::TextureManager::addTextureFromFile(textureName,fileName);
    }
    
}
void GameEngine::preloadSomeShapes()
{
    graf::ShapeCreator::create(graf::ShapeTypes::Square);
    graf::ShapeCreator::create(graf::ShapeTypes::Cube);
    graf::ShapeCreator::create(graf::ShapeTypes::Circle);            
    graf::ShapeCreator::create(graf::ShapeTypes::Pyramid);            
    graf::ShapeCreator::create(graf::ShapeTypes::Frustum);            
}
void GameEngine::sceneLoad(){
    m_window.render();
}
GameEngine::~GameEngine()
{
    delete m_scene;
}