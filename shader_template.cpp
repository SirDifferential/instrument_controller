#include "shader_template.hpp"
#include "manager.hpp"

ShaderTemplate::ShaderTemplate(const char* vs_filepath, const char* fs_filepath) : Shader(vs_filepath, fs_filepath)
{
    fprintf(stderr, "ShaderTemplate constructing\n");
}

ShaderTemplate::~ShaderTemplate()
{
}

void ShaderTemplate::update()
{
    use();
    set_int("size_x", 1024);
    set_int("size_y", 1024);
    set_float("time", manager.getTime()*10+1000);
}

void ShaderTemplate::action1()
{
}

void ShaderTemplate::action2()
{
}

void ShaderTemplate::action3()
{
}

void ShaderTemplate::action4()
{
}

void ShaderTemplate::action5()
{
}

void ShaderTemplate::action6()
{
}

void ShaderTemplate::action7()
{
}

void ShaderTemplate::action8()
{
}
