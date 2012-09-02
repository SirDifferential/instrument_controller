#ifndef __SHADER_PLASMA_HPP_
#define __SHADER_PLASMA_HPP_

#include "shader.hpp"

class ShaderPlasma : public Shader
{
private:
    int id;
public:
    ShaderPlasma(const char* vs_filepath, const char* fs_filepath);
    ~ShaderPlasma();
    
    void update();
    void action1();
    void action2();
    void action3();
    void action4();
    void action5();
    void action6();
    void action7();
    void action8();
    void synthAction(unsigned char input_data[]);
};

#endif

