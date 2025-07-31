// Basic Colour Shader

#type vertex
#version 450 core
layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aColour;
out vec4 vColour;

void main()
{
	gl_Position = aPos;
	vColour = aColour;
}

#type fragment
#version 450 core
in vec4 vColour;
out vec4 FragColor;

void main()
{
	FragColor = vColour;
}