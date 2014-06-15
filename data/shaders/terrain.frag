precision mediump float;
uniform sampler2D u_texIMG;
uniform sampler2D u_texGrass;
uniform sampler2D u_texDirt;
uniform sampler2D u_texRock;
uniform sampler2D u_texMap;
uniform sampler2D u_tex;
uniform vec3 u_fogColor;
                            
varying vec2 v_texture;
varying vec2 v_textureTile; 
varying float lerpValue;
varying vec3 v_normalW;
varying vec3 v_toEyeW;

void main()                                         
{                                                  
	vec4 grassColor = texture2D(u_texGrass, v_textureTile);
	vec4 rockColor = texture2D(u_texRock, v_textureTile);
	vec4 dirtColor = texture2D(u_texDirt, v_textureTile);
	vec4 texelColor;

	vec4 fogColor = vec4(u_fogColor, 1.0);
	float blendVal = texture2D(u_tex, v_texture).r;
	
	if(blendVal > 0.18 && blendVal < 0.3)
	{
		dirtColor *= (texture2D(u_texMap, v_texture).r + 0.1);
		texelColor = (lerpValue) * fogColor + (1.0 - lerpValue) * dirtColor;
		gl_FragColor = texelColor;
		
	}
	if(blendVal <= 0.18)
	{
		grassColor *= (texture2D(u_texMap, v_texture).r + 0.1);
		texelColor = (lerpValue) * fogColor + (1.0 - lerpValue) * grassColor;
		gl_FragColor = texelColor;

	}
	if(blendVal >= 0.3)
	{
		rockColor *= (texture2D(u_texMap, v_texture).r + 0.1);
		texelColor = (lerpValue) * fogColor + (1.0 - lerpValue) * rockColor;
		gl_FragColor = texelColor;

	}
}