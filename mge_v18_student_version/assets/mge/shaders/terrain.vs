//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D splatMap;
uniform sampler2D heightMap;
uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform	float 	heightMultiplier;
uniform	float 	time;
uniform vec3	objPosition;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out float timeS;
out vec4 textureDecider;
out vec3  worldPosition;
out vec3  vertNormal;
out float increaseHeight;
out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){
	timeS=time;
	float random = fract(sin(dot(vertex, vec3(18.6521, 88.92822, 27.9281))) * 83521.2354);
	textureDecider = texture(splatMap,uv);
	vec4 color = texture(heightMap,uv);
	increaseHeight=((color.y))*heightMultiplier;
	vec4 pos = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.f);
	vec4 newPos = pos;

	if(textureDecider.x==0&&textureDecider.y==0&&textureDecider.z==0)
	{
		increaseHeight += (cos(time)*random)*0.1;
	}

	newPos.y += increaseHeight;

	//worldPosition = newPos.xyz;
	worldPosition = vec3 (modelMatrix * vec4 (vertex, 1))+objPosition;

    gl_Position = newPos;
	texCoord = uv;

	vec2 tempuv=uv;
	tempuv.x-=0.01;
	tempuv.y+=-0.01;
	float height = texture(heightMap,tempuv).y*heightMultiplier+pos.y;
	vec3 uppperLeft = vec3(tempuv.x,height,tempuv.y);

	tempuv=uv;
	tempuv.x+=0.01;
	tempuv.y+=-0.01;
	height = texture(heightMap,tempuv).y*heightMultiplier+pos.y;
	vec3 uppperRight = vec3(tempuv.x,height,tempuv.y);

	tempuv=uv;
	tempuv.x-=0.01;
	tempuv.y-=-0.01;
	height = texture(heightMap,tempuv).y*heightMultiplier+pos.y;
	vec3 lowerLeft = vec3(tempuv.x,height,tempuv.y);

	tempuv=uv;
	tempuv.x+=0.01;
	tempuv.y-=-0.01;
	height = texture(heightMap,tempuv).y*heightMultiplier+pos.y;
	vec3 lowerRight = vec3(tempuv.x,height,tempuv.y);

	vec3 diag1 = uppperRight-lowerLeft;
	vec3 diag2 = uppperLeft-lowerRight;

    vertNormal=normalize(cross(diag1,diag2));//normal;//

    increaseHeight/=2;
}
