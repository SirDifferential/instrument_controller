#include "shader.hpp"
#include "manager.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

Shader::Shader(const char* vs_filepath, const char* fs_filepath)
{
    if (!vs_filepath || !fs_filepath)
    {
        fprintf(stderr, "Error at creating shader, missing a source file");
        manager.getEnter();
        manager.stop();
    }

    vs_source = loadSource(vs_filepath);
    fs_source = loadSource(fs_filepath);
    compile();
}

Shader::~Shader()
{
    glDeleteProgram(shader_handle);
    glDeleteShader(vs_handle);
    glDeleteShader(fs_handle);
    free(vs_source);
    free(fs_source);
}

char* Shader::loadSource(const char* filename)
{
    FILE* filePtr;
    char* buffer = NULL;

    int count = 0;

    if (filename != NULL)
    {
        filePtr = fopen(filename,"rt");
        if (filePtr != NULL)
        {
            fseek(filePtr, 0, SEEK_END);
            count = ftell(filePtr);
            rewind(filePtr);

            if (count > 0)
            {
                buffer = (char *)malloc(sizeof(char) * (count+1));
                count = fread(buffer,sizeof(char),count,filePtr);
                buffer[count] = '\0';
            }
            fclose(filePtr);
        }
    }
    return buffer;
}

void Shader::compile()
{
    fprintf(stderr, "Compiling shader\n");
    vs_handle = glCreateShader(GL_VERTEX_SHADER);
    fs_handle = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* vs_src = vs_source;
    const GLchar* fs_src = fs_source;

    glShaderSource(vs_handle, 1, &vs_src, NULL);
    glShaderSource(fs_handle, 1, &fs_src, NULL);

    glCompileShader(vs_handle);
    printShaderInfoLog(vs_handle);

    if (!(glGetError() == GL_NO_ERROR))
    {
        fprintf(stderr, "Error compiling vertex shader\n");
        manager.getEnter();
        manager.stop();
    }

    glCompileShader(fs_handle);
    printShaderInfoLog(fs_handle);

    if (!(glGetError() == GL_NO_ERROR))
    {
        fprintf(stderr, "Error compiling fragment shader\n");
        manager.getEnter();
        manager.stop();
    }

    shader_handle = glCreateProgram();
    glAttachShader(shader_handle, vs_handle);
    glAttachShader(shader_handle, fs_handle);
    glLinkProgram(shader_handle);

    printProgramInfoLog(shader_handle);
}

int Shader::printOpenglError(char* file, int line)
{
    GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void Shader::printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void Shader::printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void Shader::use()
{
    glUseProgram(shader_handle);
}

void Shader::unuse()
{
    glUseProgram(0);
}

void Shader::set_int(const char* n, int v)
{
    glUniform1i(glGetUniformLocation(shader_handle, n), v);
}

void Shader::set_float(const char* n, float v)
{
    glUniform1f(glGetUniformLocation(shader_handle, n), v);
}

void Shader::set_int_array(const char* n, int count, int* arr)
{
	glUniform1iv(glGetUniformLocation(shader_handle, n), count, arr);
}

