#ifndef __SHADER_PERLINNOISE_HPP_
#define __SHADER_PERLINNOISE_HPP_

#include "shader.hpp"
#include "sprite.hpp"
#include <memory>

class ShaderPerlinNoise : public Shader
{
private:
    int id;
    std::shared_ptr<Sprite> baseNoise;
    float persistence;
    float amplitude;
    int octaveSetter;
    float magicNumber1;
    float magicNumber2;
public:
    ShaderPerlinNoise(const char* vs_filepath, const char* fs_filepath);
    ~ShaderPerlinNoise();
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
