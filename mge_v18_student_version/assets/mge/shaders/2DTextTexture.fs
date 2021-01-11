#version 330 core
in vec2 TexCoords;
in mat4 projectionMat;

out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1,1,1,texture(text, TexCoords).r);
    color = sampled*vec4(textColor,1);//
}
