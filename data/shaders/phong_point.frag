precision mediump float;
uniform sampler2D u_texIMG; 
uniform vec4 u_cameraPosition;
uniform vec3 u_lightColor;

varying vec2 v_texture; 
varying vec3 v_normalW;
varying vec3 lightDirectionW;
varying float distToLight;
varying vec3 v_toEyeW;

void main()                                         
{
	vec4 texelColor = texture2D(u_texIMG, v_texture);
	vec4 lightColor = vec4(u_lightColor, 1.0);
	if(texelColor.a < 0.1)	
		discard;
		 
	float dotValue = max(0.0, dot(normalize(lightDirectionW), normalize(v_normalW)));
	
	vec3 R = reflect(normalize(-lightDirectionW), normalize(v_normalW));
	float specular = max(0.0, dot(normalize(R), normalize(v_toEyeW)));
	
	vec4 diffuseComponent = dotValue * lightColor * 0.1;
	vec4 specularComponent = pow(specular, 20.0) * lightColor * 0.3;
	vec4 objectComponent = (dotValue + 0.1) * texelColor;
	
	float attenuation = 1.0 / (pow(distToLight, 2.0) * 0.01 + distToLight * 0.1  + 0.5);
	
	gl_FragColor = diffuseComponent * attenuation + specularComponent * attenuation  + objectComponent * attenuation;
	//gl_FragColor = specularComponent * attenuation;
}                                                  