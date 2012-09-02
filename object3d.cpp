#include "object3d.hpp"
#include "objLoader.hpp"
#include "renderer.hpp"
#include "toolbox.hpp"

// Create a new 3D Object by loading the data from a .obj file
Object3D::Object3D(std::string path)
{
    name = path;
    loadModel(path);
    location.x = 1.0;
    location.y = 1.0;
    location.z = 0.0;
    rotation = 0;
    scale = 40.0f;
    printStats();
}

// Generate a flat plane with n subdivisions
Object3D::Object3D(int n)
{
    name = "Procedurally generated plane with subdivisions: ";
    name = tbox.combineStringAndInt(name, n);
    generatePlaneWithVerticesPerRow(n);
    location.x = 1.0;
    location.y = 1.0f;
    location.z = 0.0f;
    rotation = 0.0f;
    scale = 10.0f;
    printStats();
}

Object3D::~Object3D()
{
}

void Object3D::printStats()
{
    fprintf(stderr, "----------------\n");
    fprintf(stderr, "Object: %s\n", name.c_str());
    fprintf(stderr, "Vertices: %d\n", verticeCount);
    fprintf(stderr, "Normals: %d\n", normals.size());
    fprintf(stderr, "Indices: %d\n", indiceCount);
    fprintf(stderr, "----------------\n");
}

// Call the Object Loader to load the .obj, then fill all the required
// fields. Naturally this consists of vertices, normals and indices.
// In future, include materials and animations, stuff like that
void Object3D::loadModel(std::string path)
{
    std::vector<std::vector<double> > data;
    data = objectLoader.readObjectFile(path);
    vertices = data.at(0);
    normals = data.at(1);
    
    indices = tbox.doubleVectorToIntVector(data.at(2));
    
    verticeCount = vertices.size();
    indiceCount = indices.size();
}

int getVerticesCount(int width, int height)
{
    return width * height * 3;
}

int getIndicesCount(int width, int height)
{
    return (width*height) + (width-1)*(height-2);
}

// Influenced by http://stackoverflow.com/questions/5915753/generate-a-plane-with-triangle-strips
void Object3D::generatePlaneWithVerticesPerRow(int n)
{
    int width = n;
    int height = n;
    float spacing = 1.0f;

    // Generate the vertices of the grid
    for (int row = 0; row < width; row++)
    {
        for (int column = 0; column < height; column++)
        {
            vertices.push_back(float(column)*spacing);
            vertices.push_back(0.0f);
            vertices.push_back(float(row)*spacing);

            normals.push_back(float(column)*spacing);
            normals.push_back(0.0f);
            normals.push_back(float(column)*spacing);
        }
    }

    // Figure out the indices
    for (int row = 0; row < height-1; row++ )
    {
        if ((row & 1) == 0)
        { // even rows
            for (int col = 0; col < width; col++)
            {
                indices.push_back(col + row * width);
                indices.push_back(col + (row+1) * width);
            }
        } else
        { // odd rows
            for (int col = width-1; col > 0; col--)
            {
                indices.push_back(col + (row+1) * width);
                indices.push_back(col - 1 + + row * width);
            }
        }
    }

    verticeCount = vertices.size();
    indiceCount = indices.size();
}

// Set the location to which the object is rendered
void Object3D::setLocation(Vector3 in)
{
    location = in;
}

// Set the rotation at which the object is rendered
void Object3D::setRotation(float in)
{
    rotation = in;
}

// Call the renderer with all our data
void Object3D::render()
{
    rotation += 1.0f;
    renderer.renderObject(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, indices, vertices, normals, location, rotation, scale);
}

void Object3D::applyHeightMap(std::shared_ptr<Sprite> spritePtr)
{
    
}
