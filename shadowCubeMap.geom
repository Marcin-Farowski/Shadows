#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

// zawiera P*V[i], transformuje z WS do cubemap-face i
uniform mat4 shadowMatrices[6];

out vec4 FragPos;

void main()
{
    // iteruj po 6 œcianach cubemap
    for(int face = 0; face < 6; ++face)
    {
        // Ustawia aktualnie przetwarzany "face"
        gl_Layer = face;
        for(int i = 0; i < 3; i++)
        {
            // Przekszta³cony wierzcho³ek
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}