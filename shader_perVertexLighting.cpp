#include "shader_perVertexLighting.hpp"
#include "manager.hpp"

ShaderPVL::ShaderPVL(const char* vs_filepath, const char* fs_filepath) : Shader(vs_filepath, fs_filepath)
{
    fprintf(stderr, "ShaderPVL constructing\n");
    std::string path = "data/2D/colorMap.png";
    std::shared_ptr<Sprite> temp(new Sprite(path));
    colorMap = temp;
}

ShaderPVL::~ShaderPVL()
{
}

void ShaderPVL::update()
{
    use();
    set_int("size_x", 1024);
    set_int("size_y", 1024);
    set_float("time", manager.getTime()*10+1000);
    set_int("colorMap", 0);
    colorMap->bind(0);
}

void ShaderPVL::action1()
{
}

void ShaderPVL::action2()
{
}

void ShaderPVL::action3()
{
}

void ShaderPVL::action4()
{
}

void ShaderPVL::action5()
{
}

void ShaderPVL::action6()
{
}

void ShaderPVL::action7()
{
}

void ShaderPVL::action8()
{
}

void ShaderPVL::synthAction(unsigned char input_data[])
{
}


