precision mediump float;
uniform sampler2D u_texIMG; 
                            
varying vec2 v_texture; 
varying vec3 v_position;

void main()                                         
{                                                  
	float stepX = 1.0/1600.0;
	float stepY = 1.0/900.0;
	vec4 sum;
	
	// gaussian blur
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (-0.326212), v_texture.y + stepY * (-0.405805)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (-0.840144), v_texture.y + stepY * (-0.073580)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (-0.695914), v_texture.y + stepY * (0.457137)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (-0.203345), v_texture.y + stepY * (0.620716)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (0.962340), v_texture.y + stepY* (-0.194983)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (0.473434), v_texture.y + stepY* (-0.480026)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (0.519456), v_texture.y + stepY * (0.767022)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (0.185461), v_texture.y + stepY * (-0.893124)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (0.507431), v_texture.y + stepY * (0.064425)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (0.896420), v_texture.y + stepY * (0.412458)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (-0.321940), v_texture.y + stepY * (-0.932615)));
	sum += texture2D(u_texIMG, vec2(v_texture.x + stepX * (-0.791559), v_texture.y + stepY * (-0.597705)));
	//sum = sum / 12.0;
	
	float luminance = dot(sum.rgb, vec3( 0.299, 0.587, 0.114));
	
	gl_FragColor = floor(clamp(luminance - 10.0, 0.0, 1.0)) * sum;
}                                                  