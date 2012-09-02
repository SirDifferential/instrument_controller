#include "terrain.hpp"
#include "toolbox.hpp"
#ifdef WIN32
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif

Terrain::Terrain(int width, int length, std::string heightMap, float height)
{
    fprintf(stderr, "Creating new landscape\n");
    w = width;
    l = length;
    heightValues = tbox.giveFloatArray2D(w,l);
    normals = new Vector3*[w];
    for (int i = 0; i < l; i++)
    {
        normals[i] = new Vector3[w];
    }
    computedNormals = false;

    loadTerrain(w, l, heightMap, height);
}

Terrain::~Terrain()
{
    tbox.deleteFloatArray2D(heightValues, w);
    for (int i = 0; i < l; i++)
    {
        delete[] normals[i];
    }
    delete[] normals;
}

void Terrain::setHeight(int x, int z, float y)
{
    heightValues[x][z] = y;
    computedNormals = false;
}

float Terrain::getHeight(int x, int z)
{
    return heightValues[x][z];
}

void Terrain::computeNormals()
{
    fprintf(stderr, "Computing normals\n");
    
    if (computedNormals)
        return;

    Vector3** normals2 = new Vector3*[l];

    for (int i = 0; i < l; i++)
    {
        normals2[i] = new Vector3[w];
    }

    for (int z = 0; z < l; z++)
    {
        for (int x = 0; x < w; x++)
        {
            Vector3 sum(0.0f, 0.0f, 0.0f);
            Vector3 out;

            if (z > 0)
            {
                out = Vector3(0.0f, heightValues[z-1][x] - heightValues[z][x], -1.0f);
            }

            Vector3 in;
            if (z < l -1)
            {
                in = Vector3(0.0f, heightValues[z+1][x] - heightValues[z][x], 1.0f);
            }

            Vector3 left;
            if (x > 0)
            {
                left = Vector3(-1.0f, heightValues[z][x-1] - heightValues[z][x], 0.0f);
            }

            Vector3 right;
            if (x < w - 1)
            {
                right = Vector3(1.0f, heightValues[z][x+1] - heightValues[z][x], 0.0f);
            }

            if (x > 0 && z > 0)
            {
                sum += out.cross(left).normalize();
            }

            if (x > 0 && z < l - 1)
            {
                sum += left.cross(in).normalize();
            }

            if (x < w -1 && z < l - 1)
            {
                sum += in.cross(right).normalize();
            }

            if (x < w -1 && z > 0)
            {
                sum += right.cross(out).normalize();
            }

            normals2[z][x] = sum;
        }
    }

    const float FALLOUT_RATIO = 0.5f;
    for (int z = 0; z < l; z++)
    {
        for (int x = 0; x < w; x++)
        {
            Vector3 sum = normals2[z][x];
            if (x > 0)
            {
                sum += normals2[z][x - 1] * FALLOUT_RATIO;
            }
            if (x < w - 1)
            {
                sum += normals2[z][x + 1] * FALLOUT_RATIO;
            }
            if (z > 0)
            {
                sum += normals2[z - 1][x] * FALLOUT_RATIO;
            }
            if (z < 0)
            {
                sum += normals2[z + 1][x];
            }

            if (sum.length() == 0)
            {
                sum = Vector3(0.0, 1.0f, 0.0f);
            }
            normals[z][x] = sum;
        }
    }
    computedNormals = true;
}

Vector3 Terrain::getNormal(int x, int z)
{
    if (!computedNormals)
    {
        computeNormals();
    }
    return normals[z][x];
}

void Terrain::loadTerrain(int imageWidth, int imageHeight, std::string filename, float height)
{
    fprintf(stderr, "Loading heightmap\n");
    heightField = std::shared_ptr<Sprite>(new Sprite(filename));
    fprintf(stderr, "Setting height values\n");
    for (int y = 0; y < imageHeight; y++)
    {
        for (int x = 0; x < imageWidth; x++)
        {
            SDL_Color color = heightField->getPixel(heightField->spriteSurface, y, x);
            float h = height * ((color.r / 255.0f) - 0.5f);
            setHeight(x, y, h);
        }
    }

    fprintf(stderr, "Deleting heightmap");
    heightField.reset();
    computeNormals();
}

void Terrain::render()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

    glScalef(1,1,1);
    glTranslatef(-float(w)/2, 0.0f, -float(l)/2);
    glColor3f(0.3f, 0.9f, 0.1f);
    for (int z = 0; z < l-1; z++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(int x = 0; x < w; x++)
        {
            Vector3 normal = getNormal(x,z);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(x, getHeight(x, z), z);
            normal = getNormal(x, z+1);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(x, getHeight(x, z+1), z+1);
        }
        glEnd();
    }
}

void Terrain::update()
{
}
