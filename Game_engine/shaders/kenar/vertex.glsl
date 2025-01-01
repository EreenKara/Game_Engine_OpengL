#version 330 core

layout(location = 0) in vec3 aPos; // Başlangıç noktası

uniform mat4 uWorldTransform;
void main()
{
    gl_Position = uWorldTransform * vec4(aPos, 1.0);
}
