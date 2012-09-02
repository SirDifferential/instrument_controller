#include "shader_synth.hpp"
#include "manager.hpp"
#include "inputmanager.hpp"

ShaderSynth::ShaderSynth(const char* vs_filepath, const char* fs_filepath) : Shader(vs_filepath, fs_filepath)
{
    fprintf(stderr, "ShaderSynth constructing\n");
	synth1 = 0;
	synth2 = 0;
	synth3 = 0;
	synth4 = 0;
	synth5 = 0;
	synth6 = 0;
	synth7 = 0;
	synth8 = 0;
	synth9 = 0;
}

ShaderSynth::~ShaderSynth()
{
}

void ShaderSynth::update()
{
    use();
    set_int("size_x", 1024);
    set_int("size_y", 1024);
	set_int("synth1", synth1);
	set_int_array("downNotes", 220, manager.getInputMgr()->getSynthKeysDown());
	set_int_array("downTimes", 220, manager.getInputMgr()->getSynthDownTimes());
    set_float("time", manager.getTime()*10+1000);
}

void ShaderSynth::action1()
{
}

void ShaderSynth::action2()
{
}

void ShaderSynth::action3()
{
}

void ShaderSynth::action4()
{
}

void ShaderSynth::action5()
{
}

void ShaderSynth::action6()
{
}

void ShaderSynth::action7()
{
}

void ShaderSynth::action8()
{
}

void ShaderSynth::synthAction(unsigned char input_data[])
{
	fprintf(stderr, "Setting synth1 to %d\n", input_data[1]);
	synth1 = input_data[1];
}

