precision mediump float;                            
uniform sampler2D u_texIMG; 

varying vec2 v_texture; 

void main()                                         
{                                                  
	if(texture2D(u_texIMG, v_texture).a < 0.1)	
			discard;
	
	gl_FragColor = texture2D(u_texIMG, v_texture); 
}                                                  