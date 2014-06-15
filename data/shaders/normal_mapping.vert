uniform mat4 u_mvpMatrix;
uniform mat4 u_wMatrix;
uniform mat4 u_wMatrixInverse;
uniform sampler2D u_normalmap;
uniform vec4 u_cameraPosition;

attribute vec2 a_texture; 
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec3 a_tangent;
attribute vec3 a_bitangent;

varying vec2 v_texture; 
varying vec3 v_normalW;
varying vec3 lightDirectionT;
varying vec3 v_toEyeT;
               
void main()                                 
{                                           
   v_normalW = (u_wMatrix * vec4(a_normal, 0.0)).xyz;
   v_texture = a_texture; 
   gl_Position = u_mvpMatrix * vec4(a_position, 1.0);
   
   vec4 posW = u_wMatrix * vec4(a_position, 1.0);
   vec3 t = normalize(a_tangent);
   vec3 b = normalize(a_bitangent);
   vec3 n = normalize(cross(a_bitangent, a_tangent));
   
   mat3 tbn = mat3(t.x, b.x, n.x,
				   t.y, b.y, n.y,
				   t.z, b.z, n.z);
					   
   vec3 ld = vec3(1.0, 0.0, 0.0);
   lightDirectionT = (u_wMatrixInverse * vec4(ld, 0.0)).xyz;
   lightDirectionT = tbn * lightDirectionT;
   
   
   v_toEyeT = u_cameraPosition.xyz - posW.xyz;
   v_toEyeT = (u_wMatrixInverse * vec4(v_toEyeT, 0.0)).xyz;
   v_toEyeT = tbn * v_toEyeT;
}                                          
