#version 460 core



in uint ftype;
in uint ffacemask;

out uint facemask;
out uint type;

uniform vec3 chunkpos;

void main()
{
    type = (ftype);
    facemask = (ffacemask);
    gl_Position = vec4(vec3((gl_VertexID % 16) , floor(gl_VertexID / 256), floor((gl_VertexID % 256) / 16)) + chunkpos, 1.);
    gl_PointSize = 10;

}
