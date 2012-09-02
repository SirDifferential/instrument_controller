#include "renderer.hpp"
#include "opengl.hpp"
#include "manager.hpp"
#include "sprite.hpp"

Renderer renderer;

Renderer::Renderer()
{
    fprintf(stderr, "Renderer being hammered together from an old radio and a roll of duct tape...\n");
    eye.x = 0;
    eye.y = 0;
    eye.z = 140;
    
    center.x = 0;
    center.y = 0;
    center.z = 0;
    nativeHeight = 140.0f;
    firstRun = true;
}

Renderer::~Renderer()
{
    fprintf(stderr, "Renderer rendered renderless...\n");
}

void Renderer::moveCamera(float coord_x, float coord_y, float coord_z)
{
    eye.x = coord_x;
    eye.y = coord_y;
    eye.z = coord_z;

    center.x = coord_x;
    center.y = coord_y;
    center.z = 0;
}

void Renderer::moveForwards()
{
    eye.x += 10.0f;
}

void Renderer::moveBackwards()
{
    eye.x -= 10.0f;
}

void Renderer::moveLeft()
{
    eye.y += 10.0f;
}

void Renderer::moveRight()
{
    eye.y -= 10.0f;
}

void Renderer::panUp()
{
}

void Renderer::panDown()
{
}

void Renderer::panLeft()
{
}

void Renderer::panRight()
{
}

void Renderer::clearScreen()
{
    glClearColor(0.0,0.0,0.0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, manager.getOpengl()->getScreenX()/manager.getOpengl()->getScreenY(), 0.1, 1100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //moveCamera(sin(manager.getTime() / 3)*150, sin(manager.getTime() / 5)*150, 100);
    gluLookAt(	eye.x, eye.y, eye.z,
                center.x, center.y, center.z,
                0,1,0
              );
}

void Renderer::renderBackground()
{
}

void Renderer::renderMain()
{
    manager.getRoomMgr()->work();
}

void Renderer::renderForeground()
{
}

void Renderer::render()
{
    clearScreen();
    calculateLights();
    renderBackground();
    renderMain();
    renderForeground();
    manager.getOpengl()->swapBuffers();
}

void Renderer::renderObject(    GLenum primitiveType, int indiceCount, GLenum datatype,
                                std::vector<int> indices, std::vector<double> vertices,
                                std::vector<double> normals, Vector3 location,
                                float rotation, float scale
                            )
{
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(location.x, location.y, location.z);
    glRotatef(rotation, 1.0f, 1.0f, 1.0f);
    
    glScalef(scale, scale, scale);
    glShadeModel(GL_FLAT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    
    glVertexPointer(3, GL_DOUBLE, 0, &vertices[0]);
    glNormalPointer(GL_DOUBLE, 0, &normals[0]);
    glDrawElements(primitiveType, indiceCount, datatype, &indices[0]);
    
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    
    glPopMatrix();
}

void Renderer::calculateLights()
{
	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
}
