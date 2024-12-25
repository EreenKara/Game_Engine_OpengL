#ifndef shadermanager_hpp
#define shadermanager_hpp
#include "string"
#include "unordered_map"
#include "shaderprogram.hpp"
namespace graf
{
    class ShaderManager
    {
    private:
        static ShaderManager* m_Ins;
        static ShaderManager* getInstance();
        ShaderManager();
        std::unordered_map<std::string,ShaderProgram*> m_shaderMap;
    public:
        static ShaderProgram* getShaderProgram(const std::string& programName);
        static void addShaderFromFile(const std::string& fileName);
        ~ShaderManager();
    };
    
    
    
} // namespace graf


#endif