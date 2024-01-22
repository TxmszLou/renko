#shader vertex
#version 450 core
layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texSlot;

out vec2 v_TexCoord;
out float v_TexSlot;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * vec4(pos, 0.0, 1.0);
   v_TexCoord = texCoord;
   v_TexSlot = texSlot;
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TexSlot;

uniform vec4 u_Color;
layout (binding = 0) uniform sampler2D u_Texture1;
layout (binding = 1) uniform sampler2D u_Texture2;

void main()
{
   // sample the color from texture
   int index = int(v_TexSlot);
   if (index == 0)
      color = texture(u_Texture1, v_TexCoord);
   else
      color = texture(u_Texture2, v_TexCoord);
   // color = vec4(v_TexSlot, v_TexSlot, v_TexSlot, 1.0);
   // color = texColor;
};