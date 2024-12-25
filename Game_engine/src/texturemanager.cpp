
#define STB_IMAGE_IMPLEMENTATION
#include "texturemanager.hpp"
#include<glad/glad.h>
#include <stb/stb_image.h>
namespace graf
{
    TextureManager* TextureManager::m_Ins = nullptr;


    void TextureManager::activateTexture(const string& textureName)
    {
        auto manager = getInstance();

        if(manager->m_textureMap.count(textureName)<=0)
            return;

        unsigned int textureId=manager->m_textureMap[textureName];

        glBindTexture(GL_TEXTURE_2D, textureId); 

    }
    void TextureManager::addTextureFromFile(const string& textureName,const string& fileName)
    {
         if(getInstance()->m_textureMap.count(fileName)>0)
            return;

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        // width ve height burada stbi_load tarafından dolduruluyor.
        unsigned char *data = stbi_load(fileName.data(), &width, &height, &nrChannels, 0); 
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        getInstance()->m_textureMap[textureName]=texture;
    }
    TextureManager::TextureManager()
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    TextureManager* TextureManager::getInstance()
    {
        if(m_Ins==nullptr)
        {
            m_Ins = new TextureManager();
        }

        return m_Ins;
    }
}   