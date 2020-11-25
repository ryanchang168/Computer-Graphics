#version 430
//// TODO ////
//
// Hint:
// 1. Recieve texcoord and Normal from vertex shader
// 2. Calculate and return final color to opengl
//

uniform sampler2D Texture;

in vec3 frag_normal;
in vec2 frag_texcoord;

out vec4 outColor;

void main(){
	outColor = texture2D(Texture, frag_texcoord);
}