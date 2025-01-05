#version 330 core                      
layout (location = 0) in vec3 inPosition;  
layout (location = 1) in vec2 inTexCoord;  


uniform mat4 uWorldTransform;
uniform vec2 uTextureRepeat = vec2(1.0f,1.0f);

out vec2 texCoord;

void main()                                
{  
   // burada vec4(inposition,1.0) fonskyionundaki 1.0 Translation yani öteleme matrisindeki ötelme dogru olsun diye var.
   // egerki 1 yerien farklı bir şey olursa öteleme 3 birimse onunla çarpıalcak. orneğin vec4(ss,4.0) dedim. 3 birim de öteledim
   // o zaman öteleme 3*4'ten her bir öteleme ekseninde 4 kar olacak.

   
   gl_Position = uWorldTransform*vec4(inPosition, 1.0);    
   texCoord = inTexCoord * uTextureRepeat;
}                                          