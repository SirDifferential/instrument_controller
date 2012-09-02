#include "shader_perlinnoise.hpp"
#include "manager.hpp"

ShaderPerlinNoise::ShaderPerlinNoise(const char* vs_filepath, const char* fs_filepath) : Shader(vs_filepath, fs_filepath)
{
    // Default values are pretty good
    fprintf(stderr, "ShaderPerlinNoise constructing\n");
    baseNoise = std::shared_ptr<Sprite>(new Sprite(2048, 2048));
    amplitude = 0.45f;
    persistence = 0.85f;
    octaveSetter = 9;
    magicNumber1 = 1;
    magicNumber2 = 1;
}

ShaderPerlinNoise::~ShaderPerlinNoise()
{
}

void ShaderPerlinNoise::update()
{
    use();
    set_int("size_x", 1024);
    set_int("size_y", 1024);
    set_float("time", manager.getTime());
    set_float("persistence", persistence);  // Defines how persistent each octave is
    set_float("amplitude", amplitude);      // Defines at what amplitude each octave appears
    set_int("octaveSetter", octaveSetter);  // Defines how many octaves of noise are calculated
    set_float("magicNumber1", magicNumber1);  // Defines an offset for the noise
    set_float("magicNumber2", magicNumber2);  // Defines how much the random noise appears over the simplex noise
    baseNoise->bind(0);
    set_int("baseNoise", 0);
}

void ShaderPerlinNoise::action1()
{
    fprintf(stderr, "Changing amplitude to %f\n", amplitude);
    amplitude += 0.05f;
}

void ShaderPerlinNoise::action2()
{
    fprintf(stderr, "Changing amplitude to %f\n", amplitude);
    amplitude -= 0.05f;
}

void ShaderPerlinNoise::action3()
{
    fprintf(stderr, "Changing octaveCount to %d\n", octaveSetter);
    octaveSetter++;
}

void ShaderPerlinNoise::action4()
{
    fprintf(stderr, "Changing octaveCount to %d\n", octaveSetter);
    octaveSetter--;
}

void ShaderPerlinNoise::action5()
{
    fprintf(stderr, "Changing amplitude to %f\n", amplitude);
    amplitude += 0.5;
}

void ShaderPerlinNoise::action6()
{
    fprintf(stderr, "Changing amplitude to %f\n", amplitude);
    amplitude -= 0.5;
}

void ShaderPerlinNoise::action7()
{
    fprintf(stderr, "Changing persistence to %f\n", persistence);
    persistence += 0.05;
}

void ShaderPerlinNoise::action8()
{
    fprintf(stderr, "Changing persistence to %f\n", persistence);
    persistence -= 0.05;
}

void ShaderPerlinNoise::synthAction(unsigned char input_data[])
{
}


