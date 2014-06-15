precision mediump float;
uniform sampler2D u_texIMG;
uniform float u_innerAngle;
uniform float u_outerAngle;
uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;
                            
varying vec2 v_texture; 
varying vec3 v_normalW;
varying vec3 v_toEyeW;
varying vec3 lightDirectionW;
varying float distToLight;

void main()                                         
{                                                  
	vec4 texelColor = texture2D(u_texIMG, v_texture);
	vec4 lightColor = vec4(u_lightColor, 1.0);
	
	if(texelColor.a < 0.1)	
		discard;
			 
    vec3 spotDirection = u_lightDirection;
    
	float crt_angle = dot(normalize(-spotDirection), normalize(lightDirectionW));
	float dotValue = max(0.0, dot(normalize(lightDirectionW), normalize(v_normalW)));
	
	vec3 R = reflect(normalize(-lightDirectionW), normalize(v_normalW));
	float specular = max(0.0, dot(normalize(R), normalize(v_toEyeW)));
	
	float inner_angle = cos(u_innerAngle * (3.1415 / 180.0)); // inner angle of spot in radians
	float outer_angle = cos(u_outerAngle * (3.1415 / 180.0)); // outer angle of spot in radians
	float inner_minus_outer = inner_angle - outer_angle;
	
	float spot = clamp((crt_angle - outer_angle)/inner_minus_outer, 0.0, 1.0);
	
	vec4 diffuseComponent = spot * dotValue * lightColor * 0.3;
	vec4 specularComponent = spot * pow(specular, 20.0) * lightColor * 0.3;
	vec4 objectComponent = (spot * dotValue + 0.1) * texelColor;
	
	float attenuation = 1.0 / (pow(distToLight, 2.0) * 0.01 + distToLight * 0.1  + 0.5);
	
	gl_FragColor = attenuation * (diffuseComponent + specularComponent + objectComponent); 
}                                                  