precision mediump float;
uniform sampler2D u_texIMG;
uniform sampler2D u_depthBuffer;
uniform sampler2D u_blurredIMG;
                            
varying vec2 v_texture; 
varying vec3 v_position;

#define dof_start 0.2
#define dof_end 0.8

void main()                                         
{                                                  
	float depth = texture2D(u_depthBuffer, v_texture).r;
	
	bool x = (depth <= dof_start || depth >= dof_end);
	
	depth = float(x);
	
	gl_FragColor = (depth) * texture2D(u_blurredIMG, v_texture) +  (1.0 - depth) * texture2D(u_texIMG, v_texture);
}                                                  