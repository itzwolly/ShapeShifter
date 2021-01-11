#version 330 core
in vec4 vertex; // <vec2 pos, vec2 tex>
in vec3 normal;
in vec2 uv;
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xyz, 1.0);
    TexCoords = vertex.zw;
}