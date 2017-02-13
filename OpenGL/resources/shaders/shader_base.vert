// Version du GLSL

#version 150 core


// Entrée Shader

in vec3 in_Vertex;
in vec2 in_texCoord;

out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


// Fonction main

void main()
{
    // Position finale du vertex

    texCoord = in_texCoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Vertex, 1.0);
}
