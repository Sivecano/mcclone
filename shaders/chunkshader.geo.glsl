#version 460 core


layout(points) in;
layout(triangle_strip, max_vertices = 36) out;
out vec2 texcoord;
out float light;
in uint type[];
in uint facemask[];

const int tris[] ={
    0, 1, 2, // bottom
    1, 3, 2,

    0, 1, 4, // right
    1, 5, 4,

    1, 3, 5, // back
    3, 7, 5,

    3, 2, 7, // left
    2, 6, 7,

    2, 0, 6, // front
    0, 4, 6,

    4, 5, 7, // top
    4, 7, 6
    };

const vec3 lightdir = normalize(vec3(0,0, .1));

uniform vec3 campos;
uniform vec3 camdir;
uniform float fov;
uniform float zoom;
uniform float pitch;
uniform float yaw;

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

mat4 perpectivematrix(float fov, float distance, float depth)
{
    float n = distance;
    float r = tan(fov / 2) * distance;
    float f = distance + depth;
    float t = r * 9./16.;

    return mat4(n/r, 0, 0, 0,
                0, n/t, 0, 0,
                0, 0, -(f + n)/(depth), (2*f*n)/(depth),
                0, 0, 1, 0
                );
}

void main()
{
    float fovx = fov/2;
    float fovy = asin(sin(fovx)* 16./ 9.);
    if (type[0] == 0) return;

    vec2 blocktex = vec2((type[0]) % 48, floor((type[0]) / 48) * 16) / 16.;
    vec4 corners[8];

    // generate cube vertex in global coordinates
    for (int x = 0; x < 2; x++)
        for (int y = 0; y < 2; y++)
            for (int z = 0; z < 2; z++)
                corners[x + 2*z + 4*y] = (gl_in[0].gl_Position)  + vec4(x, y, z, 0) - vec4(0.5, 0.5, 0.5, 0);


    // transform cube vertex to screenspace coordinates
    for (int i = 0; i < 8; i++)
    {
        corners[i] -= vec4(campos, 0);
        //corners[i].xyz = inverse(mat3(normalize(cross(vec3(0,1,0), camdir)), vec3(0, 1, 0), normalize(camdir))) * corners[i].xyz;
        corners[i] = rotationMatrix(vec3(0, -1, 0), yaw + 4.7123880) * corners[i];
        corners[i] = rotationMatrix(vec3(-1,0,0), pitch) * corners[i];

        corners[i] = perpectivematrix(fov, 1 / tan(fov / 2), 400) * corners[i];
        corners[i].z *= -1;
        //corners[i].xyz /= corners[i].w;

        //corners[i].xy /= corners[i].z * tan(fovx);
        //corners[i].z = ;
        //corners[i].y *= 16./9.;
        //corners[i].z /= tan(fovx);
        //corners[i].z -= tan(fovx);
        //-((2*dist + debth) / debth + 2*dist*debth / (debth * corners[i].z));
        // corners[i].z = 1 / tan(fov/2);
        // corners[i].y *= 9./16.;
    }



    float tlight;
    for (int i = 0; i < 12; i++)
    {
        if ((uint(facemask[0] / pow(2, floor(i / 2))) % 2) == 0) continue;

        //TODO: lighting
        if (i % 2 == 0) tlight= abs(dot(cross(normalize(corners[tris[3*i + 1]].xyz - corners[tris[3*i]].xyz), normalize(corners[tris[3*i + 2]].xyz - corners[tris[3*i]].xyz)), lightdir));

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

}
