precision mediump float;                            
uniform sampler2D u_texIMG; 
uniform sampler2D u_blendMap;
uniform sampler2D u_fireMask;
uniform float u_time;

varying vec2 v_texture; 

void main()                                         
{                                                  

	vec4 disUV = texture2D(u_blendMap, vec2(v_texture.x, v_texture.y - u_time));	
	
	vec2 UV = vec2(mix(v_texture.x, disUV.x, 0.1), mix(v_texture.y, disUV.y, 0.3));
	
	if(texture2D(u_fireMask, UV).r < 0.5)
		discard;
	
	gl_FragColor = texture2D(u_texIMG, UV); 
}                                                  