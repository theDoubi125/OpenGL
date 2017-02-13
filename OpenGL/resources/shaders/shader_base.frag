#version 150 core

in vec2 texCoord;

out vec4 out_Color;

uniform mat4 transformMatrix;

uniform sampler2D text;

void main()
{
    out_Color = texture(text, texCoord);
}
