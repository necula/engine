uniform mat4 u_mvpMatrix;
uniform mat4 u_wMatrix; 
uniform sampler2D u_tex;
uniform vec4 u_cameraPosition;

attribute vec2 a_texture;
attribute vec3 a_position;
attribute vec3 a_normal;
 
varying vec2 v_texture;
varying vec2 v_textureTile; 
varying vec3 v_normalW;
varying vec3 v_toEyeW; 
varying float lerpValue; 

#define fogStart 70.0
#define fogLength 120.0
                
void main()                                 
{                                           
   vec4 height = texture2D(u_tex, a_texture);
   vec4 heightMapPosL = vec4(a_position.x, height.x * 100.0, a_position.z, 1);
   vec4 heightMapPosW = u_wMatrix * heightMapPosL;
   
   float distanceToCam = distance(u_cameraPosition.xyz, heightMapPosW.xyz);
   
   lerpValue = clamp((distanceToCam - fogStart) / fogLength, 0.0, 1.0);
   
   v_texture = a_texture;
   v_textureTile = a_texture * 64.0;
     
   gl_Position = u_mvpMatrix * heightMapPosL;
}                                          
