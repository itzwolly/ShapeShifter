//DIFFUSE COLOR VERTEX SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform	float 	time;
out vec3 newvert;
void main( void ){

	float random = fract(sin(dot(vertex, vec3(18.6521, 88.92822, 27.9281))) * 83521.2354);

	float value = time;

	newvert = vertex;
	newvert+=(cos((value*random*normal)))*0.1;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(newvert, 1.f);
}
