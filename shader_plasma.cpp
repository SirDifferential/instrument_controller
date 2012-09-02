#include "shader_plasma.hpp"
#include "manager.hpp"

ShaderPlasma::ShaderPlasma(const char* vs_filepath, const char* fs_filepath) : Shader(vs_filepath, fs_filepath)
{
    fprintf(stderr, "ShaderPlasma constructing\n");
}

ShaderPlasma::~ShaderPlasma()
{
}

void ShaderPlasma::update()
{
    set_float("time", manager.getTime());
}

void ShaderPlasma::action1()
{
}

void ShaderPlasma::action2()
{
}
void ShaderPlasma::action3()
{
}
void ShaderPlasma::action4()
{
}
void ShaderPlasma::action5()
{
}

void ShaderPlasma::action6()
{
}
void ShaderPlasma::action7()
{
}
void ShaderPlasma::action8()
{
}

void ShaderPlasma::synthAction(unsigned char input_data[])
{
}

