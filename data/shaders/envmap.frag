precision mediump float;
 
uniform sampler2D u_texIMG; 
uniform samplerCube u_skybox;                        

varying vec2 v_texture; 
varying vec4 v_normalW;
varying vec3 v_toEyeW;

void main()                                         
{                                                  
	vec3 R = reflect(normalize(-v_toEyeW), normalize(v_normalW.xyz));
	gl_FragColor = (textureCube(u_skybox, R));
}                                                  