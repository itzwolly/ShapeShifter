//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture;

layout(location = 0) out float fragmentdepth;
out vec4 fragment_color;

void main(){
    // Not really needed, OpenGL does it anyway
    fragmentdepth = gl_FragCoord.z;
    //fragment_color = vec4(1,1,1,1);
}
