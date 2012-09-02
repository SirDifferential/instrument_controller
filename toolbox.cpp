#include "toolbox.hpp"
#include "manager.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

Toolbox tbox;

// Constructs our toolbox. Sets the random seed
Toolbox::Toolbox()
{
    fprintf(stderr, "A toolbox bought from the hardware store for 4.99 euros...\n");
    timesRandomized = 0;
    srand(time(0));
}

Toolbox::~Toolbox()
{
    fprintf(stderr, "A crappy toolbox being sold on ebay...\n");
}

// Returns a random integer
int Toolbox::giveRandomInt()
{
    timesRandomized++;
    return rand();
}

// Returns a random integer in range (min, max)
int Toolbox::giveRandomInt(int min, int max)
{
    return ((rand()%(max-min))+min);
}

// Checks if the specified file exists
bool Toolbox::fileExists(std::string path)
{
    bool result = false;
    std::ifstream fileChecker(path.c_str());
    if (fileChecker)
        result = true;
    else
        result = false;
    fileChecker.close();
    return result;
}

// Searches for a file upwards from the current dir
std::string Toolbox::searchFile(int range, std::string path)
{
    fprintf(stderr, "Searching %d directories upwards for %s\n", range, path.c_str());
    std::string beginPart = "";
    std::string entirePath = path;
    if (fileExists(entirePath))
        return entirePath;
    for (int i = 1; i < range; i++)
    {
        beginPart = appendToStringNTimes("../", i);
        entirePath = beginPart;
        entirePath.append(path);
        if (fileExists(entirePath))
            return entirePath;
    }
    return "";
}

SDL_Color Toolbox::getRandomColor()
{
    int min = 1;
    int max = 255;
    SDL_Color out;
    out.r = min + (double(rand())/RAND_MAX) * ((max-min) +1);
    out.g = min + (double(rand())/RAND_MAX) * ((max-min) +1);
    out.b = min + (double(rand())/RAND_MAX) * ((max-min) +1);
    return out;
}

SDL_Color Toolbox::getBWColor(SDL_Color in)
{
    SDL_Color out;
    
    // Meh, this should work well enough
    out.r = (in.r + in.b + in.g)/3;
    out.g = out.r;
    out.b = out.r;
    
    return out;
}

std::string Toolbox::appendToStringNTimes(std::string in, int numOfTimes)
{
    std::string out;
    for (int i = 0; i < numOfTimes; i++)
    {
        out.append(in);
    }
    return out;
}

// Converts a char into a string
std::string Toolbox::charToString(char c)
{
    std::stringstream stream;
    std::string out;
    stream << c;
    stream >> out;
    
    return out;
}

std::string Toolbox::intToString(int i)
{
    std::stringstream stream;
    std::string out;
    stream << i;
    stream >> out;

    return out;
}

// Converts std::vector<double> into std::vector<int>
// Used by the objLoader
std::vector<int> Toolbox::doubleVectorToIntVector(std::vector<double> in)
{
    std::vector<int> output;
    for (int i = 0; i < in.size(); i++)
    {
        output.push_back(int(in.at(i)));
    }
    return output;
}

// Returns the resolution of x*y in a string format
std::string Toolbox::resolutionToString(int sizeX, int sizeY)
{
    std::string output;
    std::stringstream container;
    container << sizeX;
    container << "x";
    container << sizeY;
    output = container.str();
    fprintf(stderr, "resolution is: %s\n", output.c_str());
    return output;
}

std::string Toolbox::stripString(std::string source, int quantity, std::string mode)
{
    std::string output;
    if (mode.compare("fromEnd") == 0)
    {
        for (int i = 0; i < source.length()-quantity; i++)
        {
            output.append(charToString(source.at(i)));
        }
    }
    else if (mode.compare("fromBegin") == 0)
    {
        for (int i = source.length()-quantity; i < source.length(); i++)
        {
            output.append(charToString(source.at(i)));
        }
    }
    else
    {
        fprintf(stderr, "Wrong string stripping mode given, assuming fromEnd\n");
        for (int i = 0; i < source.length() -quantity; i++)
        {
            output.append(charToString(source.at(i)));
        }
    }
    return output;
}

int Toolbox::boolToInt(bool b)
{
    if (b != 0)
        return true;
    else
        return false;
}


// Reverses a string. For example: banana becomes ananab
std::string Toolbox::reverseString(std::string toReverse)
{
    std::string out = "";
    for (int i = toReverse.length()-1; i >= 0; i--)
    {
        out.append(charToString(toReverse[i]));
    }
    fprintf(stderr, "Reversed a string: \n");
    fprintf(stderr, "%s\n", toReverse.c_str());
    fprintf(stderr, "%s\n", out.c_str());
    return out;
}

// Returns a new string consisting of in1 and in2
std::string Toolbox::combineStringAndInt(std::string in1, int in2)
{
    std::string out = "";
    std::stringstream buffer;
    buffer << in1;
    buffer << in2;
    out = buffer.str();
    return out;
}

// Returns a new string consisting of in1 and in2 and in3
std::string Toolbox::combineStringAndInts(std::string in1, int in2, int in3)
{
    std::string out = "";
    std::stringstream buffer;
    buffer << in1;
    buffer << in2;
    buffer << ", ";
    buffer << in3;
    out = buffer.str();
    return out;
}

// Returns a one dimensional array of ints
int* Toolbox::giveIntArray1D(int sizeX)
{
	int* out = new int[sizeX];
	return out;
}

void Toolbox::deleteIntArray1D(int* arr)
{
	delete[] arr;
}

// Returns a two dimensional array of ints
int** Toolbox::giveIntArray2D(int sizeX, int sizeY)
{
	int** out;
	out = new int*[sizeX];
	for (int i = 0; i < sizeX; i++)
	{
		out[i] = new int[sizeY];
	}
	return out;
}

// Deletes a two dimensional array of ints
void Toolbox::deleteIntArray2D(int** arr, int sizeX)
{
	for (int i = 0; i < sizeX; i++)
		delete[] arr[i];
}

// Returns a two dimensional array of chars
unsigned char** Toolbox::giveUCharArray2D(int sizeX, int sizeY)
{
	unsigned char** out;
	out = new unsigned char*[sizeX];
	for (int i = 0; i < sizeX; i++)
	{
		out[i] = new unsigned char[sizeY];
	}
	return out;
}

// Deletes a two dimensional array of chars
void Toolbox::deleteUCharArray2D(unsigned char** arr, int sizeX)
{
	for (unsigned int i = 0; i < sizeX; i++)
		delete[] arr[i];
}

// Returns a two dimensional array of floats
float** Toolbox::giveFloatArray2D(int sizeX, int sizeY)
{
    float** out;
    out = new float*[sizeX];
    for (int i = 0; i < sizeX; i++)
    {
        out[i]= new float[sizeY];
    }
    return out;
}

// Returns a three dimensional array of floats
float*** Toolbox::giveFloatArray3D(int sizeX, int sizeY, int sizeZ)
{
    float*** out;
    out = new float**[sizeX];
    for (unsigned int i = 0; i < sizeX; i++)
    {
        out[i] = new float*[sizeY];
        for (unsigned int j = 0; j < sizeY; j++)
        {
            out[i][j] = new float[sizeZ];
        }
    }
    return out;
}

// Sets all elements in a 2D float array to 0
void Toolbox::clearFloatArray2D(float** arr, int sizeX, int sizeY)
{
    for (unsigned int i = 0; i < sizeX; i++)
    {
        for (unsigned int j = 0; j < sizeY; j++)
        {
            arr[i][j] = 0.0f;
        }
    }
}

// Sets all elements in a 3D float array to 0
void Toolbox::clearFloatArray3D(float*** arr, int sizeX, int sizeY, int sizeZ)
{
    for (unsigned int i = 0; i < sizeX; i++)
    {
        for (unsigned int j = 0; j < sizeY; j++)
        {
            for (unsigned int k = 0; k = sizeZ; k++)
            {
                arr[i][j][k] = 0.0f;
            }
        }
    }
}

void Toolbox::deleteFloatArray2D(float** arr, int sizeX)
{
    for (unsigned int i = 0; i < sizeX; i++)
        delete[] arr[i];
}

void Toolbox::deleteFloatArray3D(float*** arr, int sizeX, int sizeY)
{
    for (unsigned int i = 0; i < sizeX; i++)
    {
        for (unsigned int j = 0; j < sizeY; j++)
        {
            delete[] arr[i][j];
        }
    }
}
