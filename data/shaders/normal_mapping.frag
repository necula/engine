precision mediump float; 
uniform sampler2D u_texIMG;
uniform sampler2D u_normalmap;
                         
varying vec2 v_texture; 
varying vec3 v_normalW;
varying vec3 lightDirectionT;
varying vec3 v_toEyeT;

void main()                                         
{                                                  
	vec4 texelColor = texture2D(u_texIMG, v_texture); 
	
	vec3 normalT = normalize(texture2D(u_normalmap, v_texture).xyz * 2.0 - 1.0);
	
	float dotValue = max(0.0, dot(normalize(-lightDirectionT), normalize(normalT)));
	
	vec3 R = reflect(normalize(lightDirectionT), normalize(normalT));
	float specular = max(0.0, dot(normalize(R), normalize(v_toEyeT)));
	
	vec4 diffuseComponent = dotValue * vec4(1.0, 1.0, 1.0, 1.0) * 0.1;
	vec4 specularComponent = pow(specular, 20.0) * vec4(1.0, 1.0, 1.0, 1.0) * 0.3;
	vec4 objectComponent = (dotValue + 0.1) * texelColor;
	
	
	//gl_FragColor = specularComponent;
	gl_FragColor = diffuseComponent + specularComponent + objectComponent;
}                                                  