#shader vertex
#version 330 core
layout(location = 0) in vec2 vPos;

void main()
{
   gl_Position = vec4(vPos, 0.0, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_Color;

void main()
{
   color = u_Color;
};