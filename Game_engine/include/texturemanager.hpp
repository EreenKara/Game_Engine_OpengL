#ifndef TextureManager_hpp
#define TextureManager_hpp
#include<unordered_map>
#include<string>
#include <vector>
using namespace std;

namespace graf
{
    class TextureManager
    {
    public:
        static void activateTexture(unsigned int textureId);
        static void addTextureFromFile(const string& textureName,const string& fileName);
        static void activateTexture(const string& textureName);
    private:
        TextureManager();
        static TextureManager* m_Ins;
        static TextureManager* getInstance();
        unordered_map<string,unsigned int> m_textureMap;
    };
}

#endif