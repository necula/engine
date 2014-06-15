uniform mat4 u_mvpMatrix;
uniform mat4 u_wMatrix;
uniform vec4 u_cameraPosition;
uniform sampler2D u_tex;

attribute vec2 a_texture;
attribute vec3 a_position;
attribute vec3 a_normal;
 
varying vec2 v_texture; 
varying vec3 v_normalW;
varying vec3 v_toEyeW;

void main()                                 
{                                           
   v_normalW = (u_wMatrix * vec4(a_normal, 0.0)).xyz;
   vec4 posW = u_wMatrix * vec4(a_position, 1.0);
   
   v_toEyeW = u_cameraPosition.xyz - posW.xyz;
   
   v_texture = a_texture; 
   gl_Position = u_mvpMatrix * vec4(a_position, 1.0);
}                                          
