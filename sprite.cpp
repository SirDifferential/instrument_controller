/*
This class contains a sprite. It is created either by
1. Defining a resolution
2. Defining a path to an image file
The sprite can be used by either SDL or OpenGL. The constructor first builds an SDL_Surface,
then constructs a new glTexture2D with the pixel data from the SDL_Surface.
The default rendering function puts the texture on a flat 2D plane and maps the texture on it.
The model is then rendered at coordinates (x,y,z)

There are some operations that are performed on the sprite pixel data. After each of these
operations the glTexture2D is reseted and re-created, so the operations are somewhat heavy.
convertToGrayscale() is one of them.
*/

#include "manager.hpp"
#include "sprite.hpp"
#include "toolbox.hpp"
#include "opengl.hpp"
#include "shader.hpp"
#include <assert.h>
#include <cmath>
#include <thread>
#include <mutex>

std::mutex mutex_lock;
int threads_ready = 0;
int threads_ready2 = 0;
int number_of_threads = 8;

// Used for keeping track on things that use multithreading
void reportReady()
{
    mutex_lock.lock();
    threads_ready++;
    mutex_lock.unlock();
}

void reportReady2()
{
    mutex_lock.lock();
    threads_ready2++;
    mutex_lock.unlock();
}

Sprite::Sprite()
{
}

// Constructs a sprite with some size. Useful for shaders
Sprite::Sprite(int sizeX, int sizeY)
{
    w = sizeX;
    h = sizeY;
    x = 0;
    y = 0;
    scale = 1.0;
    id = 0;
    glGenTextures(1, &id);
    assert(id);
    opacity = 1.0f;
    name = tbox.resolutionToString(w, h);
    rotation = 0.0f;
    hasShader = false;
    shaderEnabled = false;
    
    glBindTexture(GL_TEXTURE_2D, id);
    
    GLuint target = GL_TEXTURE_2D;
    spriteSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
    
    // Get the texture format. We will want to create it according to the SDL surface
    format = checkGeneric(spriteSurface);
    
    // Lock the surface for direct pixel acccess
    SDL_LockSurface(spriteSurface);
    
    SDL_Color color;
    
    // Init the surface with random colors. Useful for debugging
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            color = tbox.getRandomColor();
            setPixel(spriteSurface, i, j, color);
        }
    }
    
    // Done, unlock
    SDL_UnlockSurface(spriteSurface);
    
    // Create the actual OpenGL texture
    glTexImage2D(target,
        0,
        format,
        spriteSurface->w,
        spriteSurface->h,
        0,
        format,
        GL_UNSIGNED_BYTE,
        spriteSurface->pixels);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    fprintf(stderr, "New sprite built with following properties: Generic sprite %s\n", name.c_str());
}

// Constructs a sprite based on a loaded image file. Good for textures
Sprite::Sprite(std::string &path) :
    x(0), y(0)
{
    x = 1;
    y = 1;
    w = 1024;
    h = 1024;
    opacity = 1;
    scale = 0.1;
    id = 0;
    glGenTextures(1, &id);
    assert(id);
    opacity = 1.0f;
    glBindTexture(GL_TEXTURE_2D, id);
    name = path;
    rotation = 0.0f;
    hasShader = false;

    col.r = 1.0f;
    col.g = 1.0f;
    col.b = 1.0f;

    GLuint target = GL_TEXTURE_2D;
    bool buildMipmaps = true;
    spriteSurface = NULL;
    fprintf(stderr, "Loading: %s\n", path.c_str());
    if (target == GL_TEXTURE_2D)
    {
        spriteSurface = IMG_Load(path.c_str());
        std::string error = IMG_GetError();
        fprintf(stderr, "IMG_Load returns: %s\n", error.c_str());
        format = check(spriteSurface, path);
        if (format == GL_RGBA)
            useAlpha = true;
        else
            useAlpha = false;
        w = spriteSurface->w;
        h = spriteSurface->h;
        
        glTexImage2D(target,
            0,
            format,
            spriteSurface->w,
            spriteSurface->h,
            0,
            format,
            GL_UNSIGNED_BYTE,
            spriteSurface->pixels);
        
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    fprintf(stderr, "New sprite built with following properties: data sprite %s, %d%s%d, use alpha: %d\n", name.c_str(), w, "x", h, useAlpha);
}

Sprite::~Sprite()
{
    assert(spriteSurface);
    fprintf(stderr, "Sprite destructing: %s\n", name.c_str());
    SDL_FreeSurface(spriteSurface);
}

// When constructing, make sure the input file is good
GLenum Sprite::check(SDL_Surface *surface, std::string &path)
{
    if (!surface) {
        fprintf(stderr, "Surface is empty: %s\n", path.c_str());
        throw std::string("SDL surface error:") += path;
    }
    
    /*if ((surface->w & (surface->w - 1)) != 0 ||
        (surface->h & (surface->h - 1)) != 0) {
        SDL_FreeSurface(surface);
        fprintf(stderr, "Image size not in power of two\n");
        throw "Image size must be in power of two";
    }
    */
    // Select textures format
    GLenum format;
    if (surface->format->BytesPerPixel == 4)
        format = GL_RGBA;
    else if (surface->format->BytesPerPixel == 3)
        format = GL_RGB;
    else if (surface->format->BytesPerPixel == 1)
        format = GL_LUMINANCE;
    else {
        SDL_FreeSurface(surface);
        throw "Image format must be 8, 24 or 32 bits per pixel.";
    }
    return format;
}

GLenum Sprite::checkGeneric(SDL_Surface* pSurface)
{
    // Select texture format
    GLenum format;
    if (pSurface->format->BytesPerPixel == 4) {
        format = GL_RGBA;
    }
    else if (pSurface->format->BytesPerPixel == 3)
    {
        format = GL_RGB;
    }
    else if (pSurface->format->BytesPerPixel == 1) {
        format = GL_LUMINANCE;
    }
    else {
        SDL_FreeSurface(pSurface);
        throw "Image format must be 8, 24 or 32 bits per pixel.";
    }
    return format;
}

// Sets one pixel of a SDL_Surface to desired colour
void Sprite::setPixel(SDL_Surface *pSurface, int x, int y, SDL_Color color)
{
    Uint32 col = SDL_MapRGB(pSurface->format, color.r, color.g, color.b);
    char* pPosition = (char*) pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);
    memcpy(pPosition, &col, pSurface->format->BytesPerPixel);
}

// Sets one pixel of a SDL_Surface to desired colour
void setPixelOffClass(SDL_Surface *pSurface, int x, int y, SDL_Color color)
{
    Uint32 col = SDL_MapRGB(pSurface->format, color.r, color.g, color.b);
    char* pPosition = (char*) pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);
    memcpy(pPosition, &col, pSurface->format->BytesPerPixel);
}

void setPixels(SDL_Surface* surface, SDL_Color col, int start_x, int stop_x, int start_y, int stop_y)
{
    for (int i = start_x; i < stop_x; i++)
    {
        for (int j = start_y; j < stop_y; j++)
        {
            setPixelOffClass(surface, i, j, col);
        }
    }
    reportReady();
}

// Set all pixels of a surface to one color
// Multithreaded
void Sprite::setAllPixels(SDL_Surface* pSurface, SDL_Color col)
{
    SDL_LockSurface(spriteSurface);
    number_of_threads = 8;
    int work_per_thread = floor(float(w) / float(number_of_threads));
    std::vector<std::thread*> threadContainer;
    threads_ready = 0;

    for (int i = 0; i < number_of_threads; i++)
    {
        std::thread pixelSetter(setPixels, pSurface, col, i*work_per_thread, (i+1)*work_per_thread, 0, h);
        std::thread* threadPtr = &pixelSetter;
        threadContainer.push_back(threadPtr);
    }

    fprintf(stderr, "Waiting for threads to finish at pixel set\n");

    while (threads_ready < number_of_threads)
    {
    }

    std::vector<std::thread*>::iterator iter;
    for (iter = threadContainer.begin(); iter != threadContainer.end(); iter++)
    {
        (*iter)->join();
    }

    SDL_UnlockSurface(spriteSurface);
    regenerateTexture();
}

// Returns a SDL_Color of a specific pixel in a SDL_Surface
SDL_Color Sprite::getPixel(SDL_Surface *pSurface, int x, int y)
{
    SDL_Color color;
    Uint32 col = 0;
    char* pPosition = (char*) pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);
    memcpy(&col, pPosition, pSurface->format->BytesPerPixel);
    SDL_GetRGB(col, pSurface->format, &color.r, &color.g, &color.b);
    return color;
}

SDL_Color getPixelOffClass(SDL_Surface *pSurface, int x, int y)
{
    SDL_Color color;
    Uint32 col = 0;
    char* pPosition = (char*) pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);
    memcpy(&col, pPosition, pSurface->format->BytesPerPixel);
    SDL_GetRGB(col, pSurface->format, &color.r, &color.g, &color.b);
    return color;
}

// Builds a new OpenGL texture2D from the SDL_Surface->pixels
void Sprite::regenerateTexture()
{
    GLuint target = GL_TEXTURE_2D;
    id = 0;
    glGenTextures(1, &id);
    assert(id);
    glBindTexture(GL_TEXTURE_2D, id);
    format = checkGeneric(spriteSurface);

    glTexImage2D(target,
        0,
        format,
        spriteSurface->w,
        spriteSurface->h,
        0,
        format,
        GL_UNSIGNED_BYTE,
        spriteSurface->pixels);
    
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

float Interpolate(float x0, float x1, float alpha)
{
   float out = x0 * (1.0 - alpha) + alpha * x1;
   return out;
}

float** Sprite::generateBaseNoise()
{
    float** baseNoise;
    baseNoise = new float*[w];
    for (int i = 0; i < w; i++)
    {
        baseNoise[i] = new float[h];
    }
    
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            baseNoise[i][j] = 1.0/float(tbox.giveRandomInt(1,100));
        }
    }

    return baseNoise;
}

void appendFinishedTexture(float*** container, int index, float** texture)
{
    mutex_lock.lock();
    container[index] = texture;
    threads_ready++;
    mutex_lock.unlock();
}

// Sets elements from start to stop to some value for array arr
// The last parameter is a pointer to the function used for doing the calculation
void calculateSmoothNoiseElements(float** arr, float** baseNoise, int start_x, int stop_x, int start_y, int stop_y, int octave, int width, int height)
{
    int samplePeriod = 1 << octave;
    float sampleFrequency = 1.0f / float(samplePeriod);

    for (int i = start_x; i < stop_x; i++)
    {
        int sample_i0 = (i / samplePeriod) * samplePeriod;
        int sample_i1 = (sample_i0 + samplePeriod) % width;
        float horizontal_blend = (i - sample_i0) * sampleFrequency;

        for (int j = start_y; j < stop_y; j++)
        {
            int sample_j0 = (j / samplePeriod) * samplePeriod;
            int sample_j1 = (sample_j0 + samplePeriod) % height;
            float vertical_blend = (j -sample_j0) * sampleFrequency;

            float top = Interpolate(baseNoise[sample_i0][sample_j0], baseNoise[sample_i1][sample_j0], horizontal_blend);
            float bottom = Interpolate(baseNoise[sample_i0][sample_j1], baseNoise[sample_i1][sample_j1], horizontal_blend);
            arr[i][j] = Interpolate(top, bottom, vertical_blend);
        }
    }
    reportReady();
}

void Sprite::generateSmoothNoise(float** baseNoise, int octave, float*** container, int width, int height)
{
    float** smoothNoise = tbox.giveFloatArray2D(width, height);

    // The computation for making the smooth noise texture is multithreaded
    // Each thread will handle a slice of the texture from some x1 to x2, and y values from 0 to the end of the array, h
    number_of_threads = 8;
    int work_per_thread = floor(float(w) / float(number_of_threads));
    std::vector<std::thread*> threadContainer;
    threads_ready = 0;

    for (int i = 0; i < number_of_threads; i++)
    {
        std::thread noiseGenerator(calculateSmoothNoiseElements, smoothNoise, baseNoise, i*work_per_thread, (i+1)*work_per_thread, 0, h, octave, w, h);
        std::thread* threadPtr = &noiseGenerator;
        threadContainer.push_back(threadPtr);
    }

    fprintf(stderr, "Waiting for threads to finish at octave %d\n", octave);

    while (threads_ready < number_of_threads)
    {
    }

    std::vector<std::thread*>::iterator iter;
    for (iter = threadContainer.begin(); iter != threadContainer.end(); iter++)
    {
        (*iter)->join();
    }

    appendFinishedTexture(container, octave, smoothNoise);
}

void mergeNoises(float amplitude, int octave, float*** smoothNoises, float** perlinNoise, 
                 SDL_Surface* out, int start_x, int stop_x, int start_y, int stop_y)
{
    SDL_Color temp;
    for (int i = start_x; i < stop_x; i++)
    {
        for (int j = start_y; j < stop_y; j++)
        {
            perlinNoise[i][j] += smoothNoises[octave][i][j] * amplitude;
            temp.r = perlinNoise[i][j]*255;
            temp.g = temp.r;
            temp.b = temp.r;
            setPixelOffClass(out, i, j, temp);
        }
    }
    reportReady();
}

SDL_Surface* Sprite::generatePerlinNoise(float** baseNoise, int octaveCount)
{
    int width = w;
    int height = h;

    // 3 dimensional container for the 2D slices of noise
    float*** smoothNoises = tbox.giveFloatArray3D(octaveCount, w, h);

    float persistence = 1.1;
 
    // Generate instances of smooth noise, based on the octave
    // They are slices in our 3D "cube". Voxels of sort

    for (int i = 0; i < octaveCount; i++)
    {
        generateSmoothNoise(baseNoise, i, smoothNoises, width, height);
    }

    float** perlinNoise = tbox.giveFloatArray2D(width, height);
    tbox.clearFloatArray2D(perlinNoise, width, height);
    float amplitude = 1.25f;
    float totalAmplitude = 0.0f;

    SDL_Color temp;
    SDL_Surface* out = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);

    number_of_threads = 8;
    int work_per_thread = floor(float(w) / float(number_of_threads));
    std::vector<std::thread*> threadContainer;
    threads_ready = 0;

    // Blend all the slices of the noise together into one 2D slice
    // This is multithreaded too. Each thread handles texture slices from some x1 to some x2
    for (int octave = octaveCount - 1; octave >= 0; octave--)
    {
        amplitude *= persistence;
        totalAmplitude += amplitude;
        threads_ready = 0;

        for (int i = 0; i < number_of_threads; i++)
        {
            std::thread noiseMerger(mergeNoises, amplitude, octave, smoothNoises, perlinNoise, out, i*work_per_thread, (i+1)*work_per_thread, 0, h);
            std::thread* threadPtr = &noiseMerger;
            threadContainer.push_back(threadPtr);
        }

        fprintf(stderr, "Waiting for threads to finish at octave %d\n", octave);

        while (threads_ready < number_of_threads)
        {
        }

        std::vector<std::thread*>::iterator iter;
        for (iter = threadContainer.begin(); iter != threadContainer.end(); iter++)
        {
            (*iter)->join();
        }
    }

    /*
    //normalisation
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            perlinNoise[i][j] /= totalAmplitude;
        }
    }
    */

    // Free some memory
    tbox.deleteFloatArray3D(smoothNoises, octaveCount, width);
    
    return out;
}

// Merges all surfaces by taking an average colour based on all the surfaces
// All surfaces should be the same size
// This is generally going to be run via multiple threads, each handling one area from start_x to stop_x, usually for full columns of y
// TODO: Highly ineffective. Some IO problems between threads? Where? Writing to the final surface? 
void mergeSurfaces(std::vector<SDL_Surface*> surfaceVector, SDL_Surface* out, int start_x, int stop_x, int start_y, int stop_y)
{
    int numberOfSurfaces = surfaceVector.size();

    std::vector<SDL_Surface*>::iterator iter;
    iter = surfaceVector.begin();
    // Get the surface size from the first surface. Assume that everything that follows is of the same size
    int sizeX = (*iter)->w;
    int sizeY = (*iter)->h;

    for (int i = start_x; i < stop_x; i++)
    {
        for (int j = start_y; j < stop_y; j++)
        {
            SDL_Color average;
            float sum1 = 0.0f;
            float sum2 = 0.0f;
            float sum3 = 0.0f;
            for (iter = surfaceVector.begin(); iter != surfaceVector.end(); iter++)
            {
                sum1 += getPixelOffClass((*iter), i, j).r;
                sum2 += getPixelOffClass((*iter), i, j).g;
                sum3 += getPixelOffClass((*iter), i, j).b;
            }
            average.r = sum1 / numberOfSurfaces;
            average.g = sum2 / numberOfSurfaces;
            average.b = sum3 / numberOfSurfaces;
            setPixelOffClass(out, i, j, average);
        }
    }
    reportReady();
}

// Merges two surfaces with specified factor
// Factor of 0: produced surface is 100% s1
// Factor of 1: produced surface is 100% s2
// Surfaces should be the same size
void mergeTwoSurfaces(SDL_Surface* s1, SDL_Surface* s2, SDL_Surface* out, float factor, bool red_only, int start_x, int stop_x, int start_y, int stop_y)
{
    for (int i = start_x; i < stop_x; i++)
    {
        for (int j = start_y; j < stop_y; j++)
        {
            SDL_Color new_colour;
            SDL_Color col1;
            SDL_Color col2;

            // Limit the factor. Without the limit this could be used for lossy amplifying of a surface, though?
            if (factor < 0.0f)
                factor = 0.0f;
            else if (factor > 1.0f)
                factor = 1.0f;

            col1 = getPixelOffClass(s1, i, j);
            col2 = getPixelOffClass(s2, i, j);
            
            // The produced colour is a weighted average: (1.0f-factor*col1) + ( (factor)*col2) where factor is in range 0...1

            new_colour.r = ((1.0f-factor)*col1.r) + ( (factor)*col2.r);

            // Optimize a bit by reducing colour look ups by 1/3 in case the image is black & white
            if (!red_only)
            {
                new_colour.g = (factor*col1.g) + ( (1.0f-factor)*col2.g);
                new_colour.b = (factor*col1.b) + ( (1.0f-factor)*col2.b);
            } else
            {
                new_colour.g = new_colour.r;
                new_colour.b = new_colour.r;
            }
            setPixelOffClass(out, i, j, new_colour);
        }
    }
    reportReady();
}

// Multithreaded blurring function
// Uses a lot of memory as the threads cannot simultaneously modify the surface,
// thus requiring writing stuff into temporary tables
// TODO: Add the rest of the colours if this works
void blurSurface(SDL_Surface* out, int range_x, int range_y, int start_x, int stop_x, int start_y, int stop_y)
{
    float** temp = tbox.giveFloatArray2D(stop_x - start_x, stop_y - start_y);
    SDL_Color col;
    col.r = 0;
    col.g = 0;
    col.b = 0;

    // Mighty Cthulhu help us... 4 for loops
    for (int i = start_x; i < stop_x; i++)
    {
        for (int j = start_y; j < stop_y; j++)
        {
            // For each pixel, do range_x*range_y kernel blur
            float sum = 0.0f;
            for (int kernel_x = -1 * range_x; kernel_x < range_x; kernel_x++)
            {
                for (int kernel_y = range_y; kernel_y > -1 * range_y; kernel_y--)
                {
                    if (i < range_x || i > stop_x || j < range_y || j > stop_y - range_y-1)
                        continue;
                    sum += getPixelOffClass(out, i+kernel_x, j+kernel_y).r;
                }
            }

            // Okay, we got the sum of all colour values for this area. Should probably do some weighting, but too lazy
            sum = sum / ((range_x*2)*(range_y*2));
            temp[i-start_x][j-start_y] = sum;
        }
    }

    reportReady2();
    fprintf(stderr, "Thread waiting: start_x: %d, stop_x: %d\n", start_x, stop_x);

    // Wait for all parts of the image to be calculated
    while (threads_ready2 < number_of_threads)
    {
    }

    // Merge the finished surface

    for (int i = start_x; i < stop_x; i++)
    {
        for (int j = start_y; j < stop_y; j++)
        {
            col.r = temp[i-start_x][j-start_y];
            col.g = col.r;
            col.b = col.r;
            setPixelOffClass(out, i, j, col);
        }
    }

    tbox.deleteFloatArray2D(temp, stop_x - start_x);
    reportReady();
}

// Filters a height map so that it makes anything below treshold into some color
// Assumes greyscale
SDL_Surface* filterMap(SDL_Surface* in, SDL_Color col, float filter)
{
    SDL_Surface* out = SDL_CreateRGBSurface(SDL_SWSURFACE, in->w, in->h, 32, 0, 0, 0, 0);
    SDL_Color temp;
    for (int i = 0; i < in->w; i++)
    {
        for (int j = 0; j < in->h; j++)
        {
            temp = getPixelOffClass(in, i, j);
            if (temp.r > filter)
                setPixelOffClass(out, i, j, col);
            else
                setPixelOffClass(out, i, j, temp);
        }
    }
    return out;
}

void Sprite::convertToGreyScale()
{
    SDL_LockSurface(spriteSurface);
    SDL_Color colorBW;
    
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            colorBW = tbox.getBWColor(getPixel(spriteSurface, i, j));
            setPixel(spriteSurface, i, j, colorBW);
        }
    }
    SDL_UnlockSurface(spriteSurface);
    regenerateTexture();
}

void Sprite::bind(int unit)
{
    assert(unit >= 0 && unit < 8);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Sprite::unbindAll()
{
    for (int i=0; i<8; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Sprite::render()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);
    glRotatef(rotation, 0, 0, 1);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    glColor4f(col.r, col.g, col.b, opacity);
    
    if (useAlpha)
    {
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	    glBlendEquation(GL_ADD);
    } else
    {
        glDisable(GL_BLEND);
    }

    if (hasShader && shaderEnabled)
    {
        shaderPtr->use();
        shaderPtr->update();
    } else
    {
        //glBindTexture(GL_TEXTURE_2D, id);
    }
    
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(-w, -h);
        glTexCoord2f(1, 1); glVertex2f(w, -h);
        glTexCoord2f(1, 0); glVertex2f(w, h);
        glTexCoord2f(0, 0); glVertex2f(-w, h);
    glEnd();

    if (hasShader && shaderEnabled)
    {
        shaderPtr->unuse();
    }
    
    glPopMatrix();
}

void Sprite::addShader(std::shared_ptr<Shader> in)
{
    shaderPtr = in;
    hasShader = true;
    assert(shaderPtr.get() != NULL);
}

void Sprite::removeShader()
{
    if (shaderPtr != NULL)
        shaderPtr->unuse();
    hasShader = false;
}

void Sprite::toggleShader()
{
    if (hasShader)
    {
        if (shaderEnabled)
        {
            shaderPtr->unuse();
            shaderEnabled = false;
        }
        else
        {
            shaderPtr->use();
            shaderEnabled = true;
        }
    }
}
