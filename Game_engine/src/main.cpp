/*
   Bir sahne ekle.
   Bu sahne sürekli kayıtlı kalsın.
   Game sınıfı bize base bir sahen versin.
   Bu sahne üzerinde kullanıcı istediği işlemleri unity gibi yapsın.
   Game sınıfı kullanıcı istersen override edebilsin onu da ayarla.
   Yani base bir game sınıfım olsun. Kamerali bir oynanabilen obje sağlasın felan filan.
   Ancak şuan hocanın bizden istediği 9 tane şekilli olan oyun base game sınıfı olmasın.
   Kullanıcı bunu kalıtım alarak yapsın.

   GUI'ın tıklananamama sorununu çöz

   Glviewport ile developer penceresi. Oyuncu penceresi yap.
   Ornegin control'e basıcna oyuncu penceresinin camerası hareket etsin alt'a basınca ekranda mosue gözüksün felan.

   Preload texture'u jsondan yap.

   Shaderprogram'ı game ve gameengine arasında ayarla. Singleton'dan çek gerekirse.
   
   ortak va batch renderin bakabilrisin. draw object içerisinde. 

   shader program'ı sildim gameengine'den bu yüzden scene'i felna ona göre ayarla.

   
   
   
   mobjects içerisinde silinen şeye ulaşmaya çalışyıor hata veriyor.
   
   
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