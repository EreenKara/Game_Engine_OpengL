<<<<<<< HEAD
#version 330 core

layout(location = 0) in vec3 aPos; // Başlangıç noktası

uniform mat4 uWorldTransform;
void main()
{
    gl_Position = uWorldTransform * vec4(aPos, 1.0);
}
=======
#version 330 core

layout(location = 0) in vec3 aPos; // Başlangıç noktası

uniform mat4 uWorldTransform;
void main()
{
    gl_Position = uWorldTransform * vec4(aPos, 1.0);
}
>>>>>>> 6cd81b1acc66ad1619ef46524d3890d6a234ea51
