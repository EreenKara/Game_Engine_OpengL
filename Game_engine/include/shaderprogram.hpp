#ifndef shaderprogram_hpp
#define shaderprogram_hpp
#include<string>
#include<unordered_map>
#include<glm/glm.hpp>
using namespace std;
namespace graf
{
    class ShaderProgram
    {
    public:
        
        void create();
        void attachShader(const string& fileName,unsigned int shadertype);
        void addUniform(const string& varName);
        void setFloat(const string& varName,float value);
        void setVec4(const string& varName,const glm::vec4& value);
        void setVec3(const string& varName,const glm::vec3& value);
        void setMat4(const string& varName,const glm::mat4& value);
        void link();
        void use();
        ~ShaderProgram();
    private:
        void deleteShaderProgram();
        string getShaderFromFile(const string& fileName);
        unsigned int m_id;
        unsigned int m_vertex_shader_id;
        unsigned int m_fragment_shader_id;

        unordered_map<string,unsigned int>  m_uniforms;
    };
}


#endif