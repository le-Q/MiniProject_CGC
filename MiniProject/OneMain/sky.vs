#version 460

layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
    TexCoord = aPos;
    vec4 pos = ProjectionMatrix * ViewMatrix * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  