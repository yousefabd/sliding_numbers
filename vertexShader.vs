#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 texCoord;
out vec2 TexCoord;
uniform mat4 transform;
void main()
{
	gl_Position = transform*aPos;
	TexCoord = texCoord;
}
