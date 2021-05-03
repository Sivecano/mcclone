#version 460 core



in uint ftype;
in uint ffacemask;

out uint facemask;
out uint type;

uniform vec3 chunkpos;
uniform vec3 campos;



void main()
{
    type = (ftype);
    facemask = (ffacemask);

    //TODO: use glVertexID to align within chunk
    gl_Position = vec4(vec3((gl_VertexID % 256) , floor(gl_VertexID / 64), 0) + chunkpos - campos + vec3(-1, .5, .7), 1.);

}
