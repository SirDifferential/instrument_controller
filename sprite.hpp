#ifndef _SPRITE_HPP_
#define _SPRITE_HPP_

// This class is a bit of a mess. I coded it based on a class by a friend of mine
// and kept on adding features as I went on, never really bothering to clean things up
// It's all public, for one. Text class, if included, uses Sprite as a parent

#include <string>
#include <vector>
#include <memory>
#ifdef WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif

#include <GL/glew.h>

class Shader;

class Sprite
{
public:

    SDL_Surface* spriteSurface;	// SDL_Surface containing the colour data
    int w, h;					// Resolution of the sprite
    float x, y, z;				// Location in the world
    float scale;                // Scaling factor
    GLuint id;					// The OpenGL handle
    GLenum format;				// The colour format (RGB, RGBA, LUMINANCE)
    float opacity;				// The transparency of the sprite
    std::string name;			// The name of our sprite. Useful for fast debugging
    Sprite();
    Sprite(int sizeX, int sizeY);
    Sprite(std::string &path);
    virtual ~Sprite();
    SDL_Color col;
    bool useAlpha;
    float rotation;
    bool hasShader;
    bool shaderEnabled;
    std::shared_ptr<Shader> shaderPtr;

    void setPixel(SDL_Surface* pSurface , int x , int y , SDL_Color color);
    SDL_Color getPixel(SDL_Surface* pSurface , int x , int y);
    void setAllPixels(SDL_Surface* pSurface, SDL_Color color);
    void convertToGreyScale();
    GLenum check(SDL_Surface *surface, std::string &path);
    GLenum checkGeneric(SDL_Surface *surface);

    void bind(int unit);
    void unbindAll();

    virtual void render();
    
    std::string getName() { return name; }
    void setWidth(int n) { w = n; }
    void setHeight(int n) { h = n; }
    int getWidth() { return w; }
    int getHeight() { return h; }
    void setName(std::string s) { name = s; }
    void setSpriteSurface(SDL_Surface* surf) { spriteSurface = surf; }
    SDL_Surface* getSpriteSurface() { return spriteSurface; }
    float** generateBaseNoise();
    void generateSmoothNoise(float** baseNoise, int octave, float*** container, int width, int height);
    SDL_Surface* generatePerlinNoise(float** baseNoise, int octaveCount);
    void createClouds();
    void regenerateTexture();
    void addShader(std::shared_ptr<Shader> in);
    void removeShader();
    void toggleShader();
};

#endif
