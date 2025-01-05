#version 330 core                                 
                                       
out vec4 fragColor;                                  
in vec2 TexCoord;
in vec4 LocalLightDirection;
in vec3 LocalPosition;
in vec3 Normal;

uniform sampler2D mySampler;
uniform vec4 uDirectionalLightColor = vec4(1.0f,1.0,1.0f,1.0f);   // isik kaynaginin rengi
uniform vec4 uAmbientLightColor= vec4(1.0f,1.0,1.0f,1.0f);       // isiklarin milyon kez  yansimasindan gelen isigin rengi
uniform vec4 uMaterialDiffuseColor = vec4(1.0f,0.0f,0.0f,1.0f);   // Cismin rengi
uniform vec4 uMaterialAmbientColor = vec4(0.7f,0.7f,0.6f,1.0f);
uniform vec4 uMaterialSpecularColor = vec4(0.7f,0.7f,0.6f,1.0f);
uniform vec4 uCameraLocalPosition;
void main()                                            
{                  
   vec3 normalizedNormal = normalize(Normal);
   vec3 normalizedLightDirection = normalize(LocalLightDirection);
   vec3 PixelToCamera  = normalize(uCameraLocalPosition-LocalPosition);
   float distance = dot(-normalizedLightDirection,normalizedNormal);


   vec4 diffuseColor = uMaterialDiffuseColor * uDirectionalLightColor *  Factor;                                    
   vec4 ambientColor = uMaterialAmbientColor * uAmbientLightColor *  uAmbientFactor;                                    
   fragColor = texture( mySampler,TexCoord)*(diffuseColor+ambientColor);                                  
}                                                      