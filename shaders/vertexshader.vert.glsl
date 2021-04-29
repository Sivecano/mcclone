#version 460 core

in vec2 position;

void main()
{
    glPosition = vec4(position, 0, 1);
}
