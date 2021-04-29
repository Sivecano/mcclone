#ifndef SHADERS_H
#define SHADERS_H

unsigned int load_shader(const char* filename, int shadertype);
unsigned int shader_program(const char *vertShaderLoc, const char *fragShaderLoc, const char* geoShaderLoc = NULL);
///TODO: learn about geometry shaders and implement dealing with them

#endif