precision mediump float;                            
varying vec2 v_texture; 
uniform sampler2D u_texIMG;
uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;

varying vec3 v_normalW;
varying vec3 v_toEyeW;

void main()                                         
{                                                  
	vec4 texelColor = texture2D(u_texIMG, v_texture);
	vec4 lightColor = vec4(u_lightColor, 1.0);
	if(texelColor.a < 0.1)	
			discard;
	
	vec3 lightDirectionW = -u_lightDirection;
	float dotValue = max(0.0, dot(normalize(lightDirectionW), normalize(v_normalW)));
	
	vec3 R = reflect(normalize(-lightDirectionW), normalize(v_normalW));
	float specular = max(0.0, dot(normalize(R), normalize(v_toEyeW)));
	
	vec4 diffuseComponent = dotValue * lightColor * 0.2;
	vec4 specularComponent = pow(specular, 20.0) * lightColor * 0.3;
	vec4 objectComponent = (dotValue + 0.1) * texelColor;
	
	//gl_FragColor = specularComponent;
	gl_FragColor = diffuseComponent + specularComponent + objectComponent;
}                                                  