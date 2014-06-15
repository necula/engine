uniform mat4 u_mvpMatrix; 

attribute vec3 a_position;

varying vec4 v_position;

void main()
{
	v_position = vec4(a_position, 1);
	gl_Position = u_mvpMatrix * vec4(a_position, 1);
	gl_Position.z = gl_Position.w - 0.0001;
}