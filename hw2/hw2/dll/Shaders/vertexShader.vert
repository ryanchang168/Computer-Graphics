#version 430

//// TODO: ////
//
// Hint:
// 1. Receive position, normal, texcoord from bind buffer
// 2. Receive Model matrix, View matrix, and Projection matrix from uniform
// 3. Pass texcoord and Normal to fragment shader (normal will not use in this homework)
// 4. Calculate view space by gl_Position (must be vec4)
//

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

out vec3 frag_normal;
out vec2 frag_texcoord;


uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

void main() {
	gl_Position = P * V * M * vec4(pos, 1.0);

	frag_normal   = normalize(normal);
	frag_texcoord = texcoord;
}
