//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D splatMap;
uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform	float 	time;

in vec3 vertex;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitangent;

out float timeS;
out vec4 textureDecider;
out vec3  worldPosition;
out vec3  vertNormal;
out vec4  vertBitangent;
out vec4  vertTangent;
out vec2 texCoord; //make sure the texture coord is interpolated
//out mat3 TBN;

void main( void ){

    vertNormal=vec3 (modelMatrix * vec4 (normal, 0));
	timeS=time;
	float random = fract(sin(dot(vertex, vec3(18.6521, 88.92822, 27.9281))) * 83521.2354);
	textureDecider = texture(splatMap,uv);
	vec4 pos = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.f);
	vertTangent = vec4(tangent, 1.f)* viewMatrix * modelMatrix;
	//vertBitangent = vec4(bitangent, 1.f)* viewMatrix * modelMatrix;
	vec3 T = normalize(vec3(modelMatrix * vec4(tangent, 0.0)));
	vec3 N = normalize(vec3(modelMatrix * vec4(vertNormal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);

	worldPosition = vec3 (modelMatrix * vec4 (vertex, 1));

    gl_Position = pos;
	texCoord = uv;
}
	
