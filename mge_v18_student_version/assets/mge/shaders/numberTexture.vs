#version 330 core
in vec4 vertex; // <vec2 pos, vec2 tex>
in vec3 normal;
in vec2 uv;
out vec2 TexCoords;
out vec2 UvCoords;
out mat4 projectionMat;

uniform mat4 projection;
uniform	mat4 mvpMatrix;

void main()
{
    gl_Position = mvpMatrix * vec4(vertex.xyz, 1.0);
    projectionMat= projection;
    TexCoords = vertex.zw;
    UvCoords=uv;
} 
