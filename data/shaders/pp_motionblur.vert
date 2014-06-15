attribute vec2 a_texture;
attribute vec3 a_position;
 
varying vec2 v_texture; 
varying vec3 v_position;

void main()                                 
{                                           
   v_texture = a_texture; 
   v_position = a_position;
   
   gl_Position = vec4(a_position, 1); 
}                                          
