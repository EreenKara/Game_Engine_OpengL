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
uniform vec4 uCameraLocalPosition  =  vec4(1.0f,1.0,1.0f,1.0f); // buradaki kamera disardan gelmeli baslangic degeri olmamali. UGrasmamak ciin yaptim simdilik.
void main()                                            
{         
   float ambientFactor = 0.4f;

   vec3 normalizedNormal = normalize(Normal);
   vec3 normalizedLightDirection = normalize(LocalLightDirection);
   vec3 PixelToCamera  = normalize(uCameraLocalPosition-LocalPosition);
   float distance = dot(-normalizedLightDirection,normalizedNormal);

   vec3 ReflectedLightDirection = normalizedNormal*2*distance + normalizedLightDirection;

   float specularCos = dot(PixelToCamera,ReflectedLightDirection);

   float specularFactor  = pow(max(specular,0.0),32);

   float lightFactor = dot(-normalizedLightDirection, normalizedNormal);


   vec4 ambientColor = uMaterialAmbientColor * uAmbientLightColor *  uAmbientFactor;                                    
   vec4 diffuseColor = uMaterialDiffuseColor * uDirectionalLightColor *  lightFactor;
   vec4 specularColor = uMaterialDiffuseColor * uMaterialSpecularColor * specularFactor;
   vec4 textureColor = texture( mySampler,TexCoord);


   fragColor = textureColor*diffuseColor*vec4(1.0f,1.0f,1.0f,1.0f);                                  
}                                                      