#include "shader_normalMap.hpp"
#include "manager.hpp"

ShaderNormalMap::ShaderNormalMap(const char* vs_filepath, const char* fs_filepath) : Shader(vs_filepath, fs_filepath)
{
    fprintf(stderr, "ShaderNormalMap onstructing\n");
    std::string path1 = "data/2D/colorMap.png";
    std::string path2 = "data/2D/normalMap.png";
    colorMap = std::shared_ptr<Sprite>(new Sprite(path1));
    normalMap = std::shared_ptr<Sprite>(new Sprite(path2));
}

ShaderNormalMap::~ShaderNormalMap()
{
}

void ShaderNormalMap::update()
{
    use();
    set_int("size_x", 1024);
    set_int("size_y", 1024);
    set_float("time", manager.getTime()*10);
    colorMap->bind(0);
    normalMap->bind(1);
    set_int("colorMap", 0);
    set_int("normalMap", 1);
}

void ShaderNormalMap::action1()
{
}

void ShaderNormalMap::action2()
{
}

void ShaderNormalMap::action3()
{
}

void ShaderNormalMap::action4()
{
}

void ShaderNormalMap::action5()
{
}

void ShaderNormalMap::action6()
{
}

void ShaderNormalMap::action7()
{
}

void ShaderNormalMap::action8()
{
}

void ShaderNormalMap::synthAction(unsigned char input_data[])
{
}

