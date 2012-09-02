#ifndef __SHADER_HPP_
#define __SHADER_HPP_

#include <GL/glew.h>
#include <string>
#include <stdio.h>

class Shader
{
private:
    int id;
    char* vs_source;
    char* fs_source;
    GLuint vs_handle;
    GLuint fs_handle;
    GLuint shader_handle;
public:
    Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
    ~Shader();

    char* loadSource(const char* filepath);
    void compile();
    int printOpenglError(char *file, int line);
    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);

    void use();
    void unuse();
    void set_int(const char* n, int v);
    void set_float(const char* n, float v);
	void set_int_array(const char* n, int count, int* array);
    virtual void update() = 0;
    virtual void action1() = 0;
    virtual void action2() = 0;
    virtual void action3() = 0;
    virtual void action4() = 0;
    virtual void action5() = 0;
    virtual void action6() = 0;
    virtual void action7() = 0;
    virtual void action8() = 0;
	virtual void synthAction(unsigned char input_data[]) = 0;
};

#endif

