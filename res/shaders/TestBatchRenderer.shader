#shader vertex
#version 450 core
layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexSlot;

// out vec2 v_TexCoord;
out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexSlot;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * vec4(a_Pos, 0.0, 1.0);
   v_Color = a_Color;
   v_TexCoord = a_TexCoord;
   v_TexSlot = a_TexSlot;
};

#shader fragment
#version 450 core

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;

out vec4 o_Color;

uniform sampler2D u_Textures[32];

void main()
{
   int texID = int(v_TexSlot);
   o_Color = texture(u_Textures[texID], v_TexCoord) * v_Color;
};