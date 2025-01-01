<<<<<<< HEAD
#version 330 core

layout(triangles) in; // Giriş olarak üçgen alır
layout(line_strip, max_vertices = 6) out; // Çıktı olarak hat şeridi

void main() {
    // İlk kenar (Vertex 0 ve Vertex 1)
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    EndPrimitive();

    // İkinci kenar (Vertex 1 ve Vertex 2)
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();

    // Üçüncü kenar (Vertex 2 ve Vertex 0)
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    EndPrimitive();
}
=======
#version 330 core

layout(triangles) in; // Giriş olarak üçgen alır
layout(line_strip, max_vertices = 6) out; // Çıktı olarak hat şeridi

void main() {
    // İlk kenar (Vertex 0 ve Vertex 1)
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    EndPrimitive();

    // İkinci kenar (Vertex 1 ve Vertex 2)
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();

    // Üçüncü kenar (Vertex 2 ve Vertex 0)
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    EndPrimitive();
}
>>>>>>> 6cd81b1acc66ad1619ef46524d3890d6a234ea51
