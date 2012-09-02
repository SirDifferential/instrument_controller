#include "shader_interference.hpp"
#include "manager.hpp"

ShaderInterference::ShaderInterference(const char* vs_filepath, const char* fs_filepath) : Shader(vs_filepath, fs_filepath)
{
    fprintf(stderr, "ShaderInterference constructing\n");
}

ShaderInterference::~ShaderInterference()
{
}

void ShaderInterference::update()
{
    use();
    set_int("size_x", 1024);
    set_int("size_y", 1024);
    set_float("time", manager.getTime()*10+1000);
}

void ShaderInterference::action1()
{
}

void ShaderInterference::action2()
{
}

void ShaderInterference::action3()
{
}

void ShaderInterference::action4()
{
}

void ShaderInterference::action5()
{
}

void ShaderInterference::action6()
{
}

void ShaderInterference::action7()
{
}

void ShaderInterference::action8()
{
}

void ShaderInterference::synthAction(unsigned char input_data[])
{
}


