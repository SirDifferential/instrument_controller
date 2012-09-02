#ifndef __SHADER_SYNTH_HPP_
#define __SHADER_SYNTH_HPP_

#include "shader.hpp"

class ShaderSynth : public Shader
{
private:
    int id;
	int synth1;
	int synth2;
	int synth3;
	int synth4;
	int synth5;
	int synth6;
	int synth7;
	int synth8;
	int synth9;
public:
    ShaderSynth(const char* vs_filepath, const char* fs_filepath);
    ~ShaderSynth();
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

