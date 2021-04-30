#version 460 core


layout(points) in;
layout(triangle_strip, max_vertices = 6) out;
out vec4 texcoord;
in float dtype[];
out float ctype;

void main()
{
    ctype = dtype[0];
    texcoord = vec4(0, 0, 0, 1);
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, -0.0, 0.0, 0.0);
    EmitVertex();

    ctype = dtype[0];
    texcoord = vec4(1, 0, 0, 1);
    gl_Position = gl_in[0].gl_Position + vec4(.1, 0.0, 0.0, 0.0);
    EmitVertex();

    ctype = dtype[0];
    texcoord = vec4(1, 1, 0, 1);
    gl_Position = gl_in[0].gl_Position + vec4(.1, 0.2, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();

    ctype = dtype[0];
    texcoord = vec4(1, 1, 0, 1);
    gl_Position = gl_in[0].gl_Position + vec4(.1, 0.2, 0.0, 0.0);
    EmitVertex();

    ctype = dtype[0];
    texcoord = vec4(0, 1, 0, 1);
    gl_Position = gl_in[0].gl_Position + vec4(-.1, 0.2, 0.0, 0.0);
    EmitVertex();

    ctype = dtype[0];
    texcoord = vec4(0, 0, 0, 1);
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, -0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
