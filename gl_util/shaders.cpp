#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

unsigned int load_shader(const char* filename, int shadertype)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    file.seekg(0, file.end); // get end

    int len = file.tellg(); // how many chars in to file?

    file.seekg(file.beg); // go back to start

    char* code = new char[len + 1]; // out buffer for shader code

    file.read(code, len); // load shader code

    code[len] = '\0';

    unsigned int out = glCreateShader(shadertype);

    glShaderSource(out, 1, &code, NULL); // buffer shader

    glCompileShader(out); // compile

    file.close(); // no longer need code source

    int  success; // shader done
    glGetShaderiv(out, GL_COMPILE_STATUS, &success);
    if(!success) // if not print error
    {
        char infoLog[512];
        glGetShaderInfoLog(out, 512, NULL, infoLog);
        printf("compilation of  %s  failed\n%s\n\n%s\n", filename, infoLog, code);
        delete[] code;
        throw std::runtime_error("shader compilation failed"); // 
    }
    delete[] code;

    return out;
}


unsigned int shader_program(const char *vertShaderLoc, const char *fragShaderLoc, const char* geoShaderLoc = NULL)
{
    unsigned int vertshader; // two shaders
    unsigned int fragshader; // two shaders
    unsigned int geoshader; // two shaders

    unsigned int out = glCreateProgram();

    vertshader = load_shader(vertShaderLoc, GL_VERTEX_SHADER);
    glAttachShader(out, vertshader);
    fragshader = load_shader(fragShaderLoc, GL_FRAGMENT_SHADER);
    glAttachShader(out, fragshader);
    if (geoShaderLoc != NULL) {
        geoshader = load_shader(geoShaderLoc, GL_GEOMETRY_SHADER);
        glAttachShader(out, geoshader);
    }

    glLinkProgram(out); // make program // done

    glDeleteShader(vertshader);
    glDeleteShader(fragshader); // no longer need those we have program
    if (geoShaderLoc != NULL)
        glDeleteShader(geoshader);

    GLint success; // error check same as load shader
    glGetProgramiv(out, GL_LINK_STATUS, &success);

    if(success == GL_FALSE) {
        char infoLog[512];
        printf("shaderporgramm linking didn't work: ");
        glGetProgramInfoLog(out, 512, nullptr, infoLog);
        printf("%s\n", infoLog);
        throw std::runtime_error("ShaderPorgram didn't work");
    }

    printf("shader linked successfully!\n");
    success = 0;
    return out;
}