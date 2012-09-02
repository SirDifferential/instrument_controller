#ifndef __SHADER_INTERFERENCE_HPP_
#define __SHADER_INTERFERENCE_HPP_

#include "shader.hpp"

class ShaderInterference : public Shader
{
private:
    int id;
public:
    ShaderInterference(const char* vs_filepath, const char* fs_filepath);
    ~ShaderInterference();
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
