#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
    TexCoord = aPos; // vec3(model * vec4(aPos, 1.0))
    vec4 pos = ProjectionMatrix * ViewMatrix * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  