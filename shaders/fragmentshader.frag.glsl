#version 460 core

#define prec float

out vec4 FragColor;
in vec2 texcoord;
in float light;

uniform sampler2D tex;


float QqhToRgb(float q1, float q2, float hue)
{
    while (hue > 360) hue -= 360;
    while (hue < 0) hue += 360;

    if (hue < 60) return q1 + (q2 - q1) * hue / 60;
    if (hue < 180) return q2;
    if (hue < 240) return q1 + (q2 - q1) * (240 - hue) / 60;
    return q1;
}

vec3 HlsToRgb(float h, float l, float s)
{
    float p2;
    if (l <= 0.5) p2 = l * (1 + s);
    else p2 = l + s - l * s;

    float p1 = 2 * l - p2;
    float float_r, float_g, float_b;
    if (s == 0)
    {
        float_r = l;
        float_g = l;
        float_b = l;
    }
    else
    {
        float_r = QqhToRgb(p1, p2, h + 120);
        float_g = QqhToRgb(p1, p2, h);
        float_b = QqhToRgb(p1, p2, h - 120);
    }

    // Convert RGB to the 0 to 255 range.

    return vec3(float_r, float_g, float_b);
}

void main()
{
    FragColor = texture(tex, texcoord.xy); //vec4(ctype / 6., 0,0, 1.);
    // FragColor.xyz *= light; //TODO: lighting
    if (FragColor.w < 0.00001)
        discard;
    return;

    vec2 pos = 2 * texcoord.xy - vec2(1, 1);


    float scale = 1;
    vec2 offset = vec2(0., 0.);

    prec x0 = pos.x;
    prec y0 = pos.y ;

    x0 = (x0 / scale + 1);
    x0 *= 1.75;
    x0 += offset.x - 2.5;
    y0 /= scale;
    y0 += offset.y;

    prec x;
    prec y;
    const int max_i = 400;
    int i = 0;

    prec tempx;
    prec x2;
    prec y2;

    prec px;
    prec py;
    prec path = 0;

    while(x2 + y2 < 4 && i < max_i)
    {
        px = x;
        py = y;
        y = 2*x*y + y0;
        x = x2 - y2 + x0;
        x2 = x*x;
        y2 = y*y;

        path += ((px -x)*(px -x) + (py -y)*(py -y));
        i++;
    }


    FragColor = vec4(HlsToRgb(float(path * path / 100) *1.5, (i == max_i)? 0 :.5, .5), 1);
}