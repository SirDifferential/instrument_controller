#ifndef __SHADER_PERVERTEXLIGHTING_HPP_
#define __SHADER_PERVERTEXLIGHTING_HPP_

#include "shader.hpp"
#include "sprite.hpp"
#include <memory>

class ShaderPVL : public Shader
{
private:
    int id;
    std::shared_ptr<Sprite> colorMap;
public:
    ShaderPVL(const char* vs_filepath, const char* fs_filepath);
    ~ShaderPVL();
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
