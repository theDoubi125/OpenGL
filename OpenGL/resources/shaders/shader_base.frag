#version 150 core

in vec2 texCoord;

out vec4 out_Color;

uniform mat4 transformMatrix;

uniform sampler2D text1;
uniform sampler2D text2;

void main()
{
    out_Color = mix(texture(text1, texCoord), texture(text2, texCoord), 0.5);
}
