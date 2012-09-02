#ifndef _ROOM_HPP_
#define _ROOM_HPP_

#include "sprite.hpp"
#include "object3d.hpp"
#include "terrain.hpp"
#include <map>
#include <memory>

typedef std::map<std::string, std::shared_ptr<Sprite> > spriteContainer;
typedef std::map<std::string, std::shared_ptr<Object3D> > modelContainer;
typedef std::map<std::string, std::shared_ptr<Terrain> > terrainContainer;
class Room
{
private:
    std::string name;
    int roomNumber;
    int roomType;

    spriteContainer spriteMap;
    spriteContainer::iterator spriteIterator;

    modelContainer modelMap;
    modelContainer::iterator modelIterator;

    terrainContainer terrainMap;
    terrainContainer::iterator terrainIterator;

    double roomStartTime;
    bool firstRun;
    double timePerPicture; // For cinematics
    double pictureChangeTime; // For cinematics
    std::string music;

    std::shared_ptr<Sprite> worldSprite;

public:
    Room(std::string s, std::string mus, int n, int t);
    ~Room();
    void operate();
    std::shared_ptr<Sprite> getSprite(std::string name);
    void addSprite(std::string name, std::shared_ptr<Sprite> s);
    void removeSprite(std::string name);
    void iterateSprites();
    void toggleShaders();
    void shaderAction1();
    void shaderAction2();
    void shaderAction3();
    void shaderAction4();
    void shaderAction5();
    void shaderAction6();
    void shaderAction7();
    void shaderAction8();
	void synthAction(unsigned char input_data[]);

    void addModel(std::string name, std::shared_ptr<Object3D> m);
    void removeModel(std::string name);

    void addTerrain(std::string name, std::shared_ptr<Terrain> t);
    void removeTerrain(std::string name);

    std::shared_ptr<Sprite> findSprite(std::string f);
    std::shared_ptr<Object3D> findModel(std::string f);
    std::shared_ptr<Terrain> findTerrain(std::string f);

    std::string getName() { return name; }
    int getType() { return roomType; }
};

#endif

