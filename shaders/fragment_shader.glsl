#version 430 core

uniform vec4 out_color;
out vec4 out_color_actual;
void main()
{
	out_color_actual = out_color;
}
