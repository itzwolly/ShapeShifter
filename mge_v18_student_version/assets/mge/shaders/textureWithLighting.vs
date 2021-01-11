//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated
out vec3  vertNormal;
out vec3  worldPosition;

void main( void ){
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.f);
	worldPosition = vec3 (modelMatrix * vec4 (vertex, 1));
	vertNormal=vec3 (modelMatrix * vec4 (normal, 0));//the 0 ignores translations from the modelMatrix;
	texCoord = uv;

}
