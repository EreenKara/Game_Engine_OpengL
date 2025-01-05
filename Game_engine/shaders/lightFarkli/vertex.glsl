#version 330 core       

// Vertex shader'dan Fragment shader'a gidene kadar vertex shader'daki bu noktalar fragment shader'daki
// pixellerin bulunması için kullanılıyor.
// Noktalara intepolation islemi uygulanarak pixeller bulunuyorlar.
// Sonucta ben 3 nokta vererek ucgen cizdiriyorum ancak ucgen alani icerisinde 3'ten fazla pixel bulunuyor.
// Bu pixellerin hesaplanması bu 3 nokta kullanılarak oluyor yani.  

layout (location = 0) in vec3 inPosition;  
layout (location = 1) in vec2 inTexCoord;  
layout (location = 2) in vec3 inNormal;  

uniform mat4 uWorldTransform;  
uniform vec3 uLightDirection = vec3(0.0f,0.0f,1.0f);
uniform vec2 uTextureRepeat = vec2(1.0f,1.0f);
uniform mat4 uInverseRotation;

out vec4 LocalLightDirection;
out vec2 TexCoord;
out vec3 Normal;
out vec3 LocalPosition;
void main()                                
{              
   LocalLightDirection = uInverseRotation*vec4(uLightDirection,0.0f);   
   gl_Position = uWorldTransform*vec4(inPosition, 1.0);
   Normal = inNormal;
   LocalPosition = inPosition;
   TexCoord = inTexCoord * uTextureRepeat;    
}                                          