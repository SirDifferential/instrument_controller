#ifndef _TOOLBOX_HPP_
#define _TOOLBOX_HPP_

#include <string>
#include <vector>
#ifdef WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#endif

class Toolbox
{
private:
public:
    Toolbox();
    ~Toolbox();
    
    int timesRandomized;
    
    int giveRandomInt();
    int giveRandomInt(int min, int max);
    
    bool fileExists(std::string path);
    SDL_Color getRandomColor();
    SDL_Color getBWColor(SDL_Color in);
    
    std::string searchFile(int range, std::string path);
    
    std::string appendToStringNTimes(std::string in, int numOfTimes);
    std::string charToString(char c);
    std::string intToString(int i);
    std::string resolutionToString(int sizeX, int sizeY);
    int boolToInt(bool b);
    std::vector<int> doubleVectorToIntVector(std::vector<double> in);
    std::string stripString(std::string source, int quantity, std::string mode);
    std::string relativePathToAbsolute(std::string path);
    std::string reverseString(std::string toReverse);
    std::string combineStringAndInt(std::string in1, int in2);
    std::string combineStringAndInts(std::string in1, int in2, int in3);

	
	int* giveIntArray1D(int sizeX);
	void deleteIntArray1D(int* arr);
	int** giveIntArray2D(int sizeX, int sizeY);
	void deleteIntArray2D(int** arr, int sizeX);
	unsigned char** giveUCharArray2D(int sizeX, int sizeY);
	void deleteUCharArray2D(unsigned char** arr, int sizeX);
    float** giveFloatArray2D(int sizeX, int sizeY);
    float*** giveFloatArray3D(int sizeX, int sizeY, int sizeZ);
    void clearFloatArray2D(float** arr, int sizeX, int sizeY);
    void deleteFloatArray2D(float** arr, int sizeX);
    void clearFloatArray3D(float*** arr, int sizeX, int sizeY, int sizeZ);
    void deleteFloatArray3D(float*** arr, int sizeX, int sizeY);
};

extern Toolbox tbox;

#endif

