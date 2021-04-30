#version 460 core

in float type;
out float dtype;

void main()
{
    dtype = type;
    gl_Position = vec4((gl_VertexID % 3) / 1.5 - .7, floor(gl_VertexID / 3) - .55, 0, 1.);

}
