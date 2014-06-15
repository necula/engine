precision mediump float;                            
uniform sampler2D u_texIMG; 

varying vec2 v_texture; 
varying vec3 v_position;

#define vignette_start 0.9
#define vignette_length 0.2

void main()                                         
{                                                  
	float dist = distance(v_position, vec3(0.0, 0.0, 0.0));
	dist = abs(clamp((dist - vignette_start) / vignette_length, 0.0, 1.0) - 1.0);
	
	float lum = dot(texture2D(u_texIMG, v_texture).rgb, vec3(0.3, 0.59, 0.11));
	
	gl_FragColor = vec4(vec3(lum), 1.0) * dist; 
}                                                  