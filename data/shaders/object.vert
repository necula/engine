uniform mat4 u_mvpMatrix;
 
attribute vec2 a_texture; 
attribute vec3 a_position;
attribute vec3 a_normal;                

varying vec2 v_texture; 

void main()                                 
{                                           
   v_texture = a_texture; 
   gl_Position = u_mvpMatrix * vec4(a_position, 1); 
}                                          
