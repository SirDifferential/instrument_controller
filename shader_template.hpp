#ifndef __SHADER_TEMPLATE_HPP_
#define __SHADER_TEMPLATE_HPP_

#include "shader.hpp"

class ShaderTemplate : public Shader
{
private:
    int id;
public:
    ShaderTemplate(const char* vs_filepath, const char* fs_filepath);
    ~ShaderTemplate();
    void update();
    void action1();
    void action2();
    void action3();
    void action4();
    void action5();
    void action6();
    void action7();
    void action8();
};

#endif
