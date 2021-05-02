#version 460 core


layout(points) in;
layout(triangle_strip, max_vertices = 36) out;
out vec2 texcoord;
out float light;
in uint type[];
in uint facemask[];
const int tris[] ={
    0, 1, 2,
    1, 3, 2,

    0, 1, 4,
    1, 5, 4,

    1, 3, 5,
    3, 7, 5,

    3, 2, 7,
    2, 6, 7,

    2, 0, 6,
    0, 4, 6,

    4, 5, 7,
    4, 7, 6
    };

const vec3 lightdir = normalize(vec3(0,0, .1));

uniform vec3 camdir;
uniform float fov;

uniform int time;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
    oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
    oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
    0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    vec2 blocktex = vec2((type[0]) % 48, floor((type[0]) / 48) * 16) / 16.;

    float d = 0.1 / gl_in[0].gl_Position.z;
    vec4 origin = gl_in[0].gl_Position - vec4(d, d, d, 0);

    vec4 corners[12];

    for (int x = 0; x < 2; x++)
        for (int y = 0; y < 2; y++)
            for (int z = 0; z < 2; z++)
            {
                corners[x + 2*z + 4*y] = gl_in[0].gl_Position  +
                                        vec4(1, 16./9., 1, 1) *
                                    (rotationMatrix(vec3(.2, 1, .1), time / 500.) *
                                    (vec4((2 * x - 1)*d, (2 * y - 1)*d, (2 * z - 1)*d, 1.)));
            }
    float tlight;
    for (int i = 0; i < 12; i++)
    {
        if ((uint(facemask[0] / pow(2, floor(i / 2))) % 2) == 0) continue;

        //TODO: lighting
        //if (i % 2 == 0)
        //    tlight= abs(dot(cross(normalize(corners[tris[3*i + 1]].xyz - corners[tris[3*i]].xyz), normalize(corners[tris[3*i + 2]].xyz - corners[tris[3*i]].xyz)), lightdir));

        for (int j = 0; j < 3; j++)
        {
            int index = tris[i * 3 + j];
            gl_Position = corners[index];
            texcoord = blocktex + vec2(1. / 48 + int((index % 4) % 3 == 0) / 48., int(index < 4));

            if (i < 2 || i > 9)
                texcoord = blocktex + vec2(int(index % 2 == 1) /48. + int(index < 4) * 2./48. , int((index % 4) > 1));

            light = tlight;

            EmitVertex();
        }
        EndPrimitive();
    }


/*
    // ctype = dtype[0];
    texcoord = vec2((dtype[0] - 1.) / 16., 1);
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, -0.0, 0.0, 0.0);
    EmitVertex();

    // ctype = dtype[0];

    texcoord = vec2(dtype[0] / 16., 1);
    gl_Position = gl_in[0].gl_Position + vec4(.1, 0.0, 0.0, 0.0);
    EmitVertex();

    // ctype = dtype[0];
    texcoord = vec2(dtype[0] / 16., 0);
    gl_Position = gl_in[0].gl_Position + vec4(.1, 0.2, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();

    // ctype = dtype[0];
    texcoord = vec2(dtype[0] / 16., 0);
    gl_Position = gl_in[0].gl_Position + vec4(.1, 0.2, 0.0, 0.0);
    EmitVertex();

    // ctype = dtype[0];
    texcoord = vec2((dtype[0] - 1.)/16., .0);
    gl_Position = gl_in[0].gl_Position + vec4(-.1, 0.2, 0.0, 0.0);
    EmitVertex();

    // ctype = dtype[0];
    texcoord = vec2(dtype[0] / 16. - 1. / 16., 1);
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, -0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();*/
}
