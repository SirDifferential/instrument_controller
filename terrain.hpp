#ifndef __TERRAIN_HPP_
#define __TERRAIN_HPP_

// http://www.videotutorialsrock.com/opengl_tutorial/terrain/text.php

#include "vector3.hpp"
#include "sprite.hpp"
#include <memory>

class Terrain
{
private:
    int id;
    int w;
    int l;
    float** heightValues;
    Vector3** normals;
    bool computedNormals;
    std::shared_ptr<Sprite> heightField;
public:
    Terrain(int a, int b, std::string filePath, float height);
    ~Terrain();
    void setHeight(int x, int z, float y);
    float getHeight(int x, int z);
    void computeNormals();
    Vector3 getNormal(int x, int z);
    void loadTerrain(int a, int b, std::string filename, float height);

    void render();
    void update();

    int getWidth() { return w; }
    int getLength() { return l; }
};

#endif
