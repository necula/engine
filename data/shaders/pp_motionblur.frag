precision mediump float;
uniform sampler2D u_texIMG;
uniform sampler2D u_depthBuffer;
uniform mat4 u_previousViewProjection;
uniform mat4 u_viewProjectionInverse;
                            
varying vec2 v_texture; 
varying vec3 v_position;

#define dof_start 0.2
#define dof_end 0.8

void main()                                         
{                                                  
	
	vec2 texcoords = v_texture;
	float zOverW = texture2D(u_depthBuffer, v_texture).r;
	vec4 H = vec4(v_texture.x * 2.0 - 1.0, (1.0 - v_texture.y) * 2.0 - 1.0, zOverW, 1.0);
	vec4 D = u_viewProjectionInverse * H;
	vec4 worldPos = D / D.w;
	
	vec4 currentPos = H;
	
	vec4 previousPos = u_previousViewProjection * worldPos;
	previousPos = previousPos / previousPos.w;
	
	vec2 velocity = (currentPos.xy - previousPos.xy) / 2.0;
	 
	vec4 color = texture2D(u_texIMG, v_texture);
	texcoords += velocity;

	for(int i = 1; i < 2; ++i, texcoords += velocity)
	{
		vec4 currentColor = texture2D(u_texIMG, texcoords);
		
		color += currentColor;
	}
	
	vec4 finalColor = color / 2.0;
	
	gl_FragColor = finalColor;
}                                                  