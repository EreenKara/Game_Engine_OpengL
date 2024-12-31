#version 330 core

layout(triangles) in;           // Üçgenler input olarak alınır
layout(line_strip, max_vertices = 6) out; // Çizgi modunda çıktılar oluşturulur

in vec3 fragColor[]; // Vertex Shader'den gelen renk verisi
out vec3 geomColor;  // Fragment Shader'a gönderilecek renk

void main() {
    // Her üçgenin 3 kenarını sırayla çizin
    for (int i = 0; i < 3; ++i) {
        vec4 v0 = gl_in[i].gl_Position;              // Şu anki vertex
        vec4 v1 = gl_in[(i + 1) % 3].gl_Position;    // Sonraki vertex

        // İlk vertex pozisyonunu çiz
        gl_Position = v0;
        geomColor = fragColor[i];
        EmitVertex();

        // İkinci vertex pozisyonunu çiz
        gl_Position = v1;
        geomColor = fragColor[(i + 1) % 3];
        EmitVertex();

        EndPrimitive(); // Her çizgi için primitiv'i bitir
    }
}