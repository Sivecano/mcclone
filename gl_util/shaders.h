#ifndef SHADERS_H
#define SHADERS_H

unsigned int load_shader(const char* filename, int shadertype);
unsigned int shader_program(const char *vertShaderLoc, const char *fragShaderLoc, const char* geoShaderLoc = NULL);
unsigned int compute_shader(const char* computeShaderLoc);

#endif