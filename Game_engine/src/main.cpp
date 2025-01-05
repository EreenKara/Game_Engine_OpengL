/*
  
   Glviewport ile developer penceresi. Oyuncu penceresi yap.
   Ornegin control'e basıcna oyuncu penceresinin camerası hareket etsin alt'a basınca ekranda mosue gözüksün felan.

   ortak va batch renderin bakabilrisin. draw object içerisinde. 

    Ortak batch kullanımı dediği aynı shader'ı kullanan objeleri belirle. Ortak VA'yı kullananları belirle. 
    ORtak texture kulalnanları belirle. Bunları ekrana belirli hiyerarşi de çiz. Bir kere texture'ı yükleyince
    o texture'yi kullanan tüm cisimleri ekrana çizdir.

    Normalde çizim yapılırken sadece kullanıcının görebileceği kısım çizdirilir. Arkada kalan kısım çizdirilmez.

    Ekrandaki imgui kapanıp açıldıkta sonra düzgün active object'i göstermiyor onu ayarla.
    Scene içerisinden Imgui'i çıkartıp daha modüler yap.


    Save'den yükelme esnasında cosntructor'ların doğru olup olmadığına ve daha iyi nasıl yapılır ona bak.
*/


#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include "glwindow.hpp"
#include "shaderprogram.hpp"
#include "vertexarrayobject.hpp"
#include "texturemanager.hpp"
#include "shapecreator.hpp"
#include <iostream>
#include <iomanip>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gameengine.hpp"
#include "scene.hpp"

using namespace std;

int main(int arc,char** argv)
{

    GameEngine gameEngine;
    gameEngine.sceneLoad();
    exit(EXIT_SUCCESS);
}