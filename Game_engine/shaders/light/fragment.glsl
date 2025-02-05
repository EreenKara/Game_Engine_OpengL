#version 330 core                                 
                                       
out vec4 fragColor;                                  
in vec2 TexCoord;
in float Factor;  // teta açısı, isik yonu ile normalden hesaplanan factor. Yani bir bakima yuzeye uygulanacak siddet. 
uniform sampler2D mySampler;
uniform vec4 uDirectionalLightColor = vec4(1.0f,1.0,1.0f,1.0f);   // isik kaynaginin rengi
uniform vec4 uMaterialDiffuseColor = vec4(1.0f,1.0f,1.0f,1.0f);   // Cismin rengi
uniform vec4 uMaterialAmbientColor = vec4(1.0f,1.0f,1.0f,1.0f);
uniform vec4 uAmbientLightColor= vec4(1.0f,1.0,1.0f,1.0f);       // isiklarin milyon kez  yansimasindan gelen isigin rengi
uniform float uAmbientFactor = 0.4f;
void main()                                            
{                  

   vec4 diffuseColor = uMaterialDiffuseColor * uDirectionalLightColor *  Factor;                                    
   vec4 ambientColor = uMaterialAmbientColor * uAmbientLightColor *  uAmbientFactor;                                    
   fragColor = texture( mySampler,TexCoord) * (diffuseColor+ambientColor);                                  
}                                                      