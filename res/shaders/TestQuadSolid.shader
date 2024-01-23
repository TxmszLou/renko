#shader vertex
#version 450 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
out float v_TexSlot;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * vec4(pos, 0.0, 1.0);
   v_TexCoord = texCoord;
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
   // sample the color from texture
   color = u_Color;
   // color = vec4(v_TexSlot, v_TexSlot, v_TexSlot, 1.0);
   // color = texColor;
};