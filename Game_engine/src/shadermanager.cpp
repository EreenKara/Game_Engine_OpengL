#include "shadermanager.hpp"
#include "nlohmann/json.hpp"
#include "fstream"
#include <iostream>
#include "shaderprogram.hpp"
#include "glwindow.hpp"


namespace graf
{
    ShaderManager* ShaderManager::m_Ins = nullptr;
    ShaderManager::ShaderManager()
    {
        
    }
    ShaderManager* ShaderManager::getInstance()
    {
        if(m_Ins==nullptr)
        {
            m_Ins = new ShaderManager();
        }
        return m_Ins;
    }
    void ShaderManager::addShaderFromFile(const std::string& fileName)
    {
        auto manager = getInstance();
        std::ifstream file("./shaders/"+fileName);
        auto data = nlohmann::json::parse(file);
        
        std::string programName = data["ProgramName"].get<std::string>();        
        std::string vertexShaderName = data["VertexShader"].get<std::string>();        
        std::string fragmentShaderName = data["FragmentShader"].get<std::string>();        
        auto uniform = data["Uniforms"];        


        graf::ShaderProgram* shaderProgram = new graf::ShaderProgram();
        shaderProgram->create();
        shaderProgram->attachShader("./shaders/"+vertexShaderName,GL_VERTEX_SHADER);
        shaderProgram->attachShader("./shaders/"+fragmentShaderName,GL_FRAGMENT_SHADER);
        shaderProgram->link();
        for(auto siradaki:uniform)
        {
            shaderProgram->addUniform(siradaki["Name"].get<std::string>());
        }
        
        manager->m_shaderMap[programName] = shaderProgram;
    }
    ShaderProgram* ShaderManager::getShaderProgram(const std::string& programName)
    {
        auto manager = getInstance();
        if(manager->m_shaderMap.count(programName)>0)
        {
            return manager->m_shaderMap[programName];
        }
        return nullptr;
    }

    ShaderManager::~ShaderManager()
    {
    }
} // namespace graf
