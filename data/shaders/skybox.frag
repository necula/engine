precision mediump float;
uniform samplerCube u_texture;

varying vec4 v_position;

void main()
{
	
	gl_FragColor = textureCube(u_texture, v_position.xyz);// / 2.0;
}                                         