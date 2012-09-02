#include "room.hpp"
#include "manager.hpp"
#include "renderer.hpp"
#include "shader.hpp"
//#include "shader_perlinNoise.hpp"
//#include "shader_interference.hpp"

Room::Room(std::string s, std::string mus, int n, int t)
{
    fprintf(stderr, "Room creating with name %s and number %d and type %d\n", s.c_str(), n, t);
    name = s;
    roomNumber = n;
    roomType = t;
    music = mus;
    firstRun = true;
}

Room::~Room()
{
}

std::shared_ptr<Sprite> Room::findSprite(std::string toSearch)
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if (((*spriteIterator).first.compare(toSearch) == 0))
        {
            return (*spriteIterator).second;
        }
    }
    fprintf(stderr, "No such sprite %s in room %d\n", toSearch.c_str(), roomNumber);
    return std::shared_ptr<Sprite>();
}

void Room::operate()
{
	if (roomType == 1)	// An arena
	{
        if (firstRun)
        {
            firstRun = false;
        }

        for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
            (*spriteIterator).second->render();

        for (modelIterator = modelMap.begin(); modelIterator != modelMap.end(); modelIterator++)
            (*modelIterator).second->render();

        for (terrainIterator = terrainMap.begin(); terrainIterator != terrainMap.end(); terrainIterator++)
            (*terrainIterator).second->render();
	} else if (roomType == 3) // Cinematics
    {
        renderer.moveCamera(0, 0, renderer.getNativeHeight());
        if (firstRun)
        {
            roomStartTime = manager.getTime();
            timePerPicture = 4;
            pictureChangeTime = roomStartTime;
            spriteIterator = spriteMap.begin();
            firstRun = false;
        }

        if (manager.getTime() - pictureChangeTime > timePerPicture)
        {
            pictureChangeTime = manager.getTime();
            spriteIterator++;
        }
        if (spriteIterator == spriteMap.end())
            manager.getRoomMgr()->nextRoom();
        else
            (*spriteIterator).second->render();
	} else if (roomType == 4) // Static text rooms
    {
        renderer.moveCamera(0, 0, renderer.getNativeHeight());
        if (firstRun)
        {
            spriteIterator = spriteMap.begin();
            firstRun = false;
        }

        if (spriteIterator == spriteMap.end())
            manager.getRoomMgr()->nextRoom();
        else
            (*spriteIterator).second->render();
    }
}

std::shared_ptr<Sprite> Room::getSprite(std::string name)
{
    spriteIterator = spriteMap.find(name.c_str());
    if (spriteIterator != spriteMap.end())
    {
        return spriteMap[name.c_str()];
    } else
    {
        fprintf(stderr, "Requested sprite not found! Sprite: %s, room: %d", name.c_str(), roomNumber);
        std::shared_ptr<Sprite> nullPointer;
        return nullPointer;
    }
}

void Room::addSprite(std::string name, std::shared_ptr<Sprite> s)
{
    spriteMap[name] = s;
}

void Room::removeSprite(std::string name)
{
    spriteIterator = spriteMap.find(name.c_str());
    if (spriteIterator != spriteMap.end())
    {
        spriteMap.erase(spriteIterator);
    } else
    {
        fprintf(stderr, "No sprite found to delete! Sprite: %s, Room: %d", name.c_str(), roomNumber);
    }
}

void Room::iterateSprites()
{
    spriteIterator++;
}

void Room::addModel(std::string name, std::shared_ptr<Object3D> m)
{
    modelMap[name] = m;
}

void Room::removeModel(std::string name)
{
    modelIterator = modelMap.find(name.c_str());
    if (modelIterator != modelMap.end())
    {
        modelMap.erase(modelIterator);
    } else
    {
        fprintf(stderr, "No model found to delete! Sprite: %s, Room: %d\n", name.c_str(), roomNumber);
    }
}

void Room::addTerrain(std::string name, std::shared_ptr<Terrain> t)
{
    terrainMap[name] = t;
}

void Room::removeTerrain(std::string name)
{
    terrainIterator = terrainMap.find(name.c_str());
    if (terrainIterator != terrainMap.end())
    {
        terrainMap.erase(terrainIterator);
    } else
    {
        fprintf(stderr, "No terrain found to delete! Terrain %s, Room %d\n", name.c_str(), roomNumber);
    }
}

void Room::toggleShaders()
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->toggleShader();
        }
    }
}

void Room::shaderAction1()
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->shaderPtr->action1();
        }
    }
}

void Room::shaderAction2()
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->shaderPtr->action2();
        }
    }
}

void Room::shaderAction3()
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->shaderPtr->action3();
        }
    }
}

void Room::shaderAction4()
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->shaderPtr->action4();
        }
    }
}

void Room::shaderAction5()
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->shaderPtr->action5();
        }
    }
}

void Room::shaderAction6()
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->shaderPtr->action6();
        }
    }
}

void Room::shaderAction7()
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->shaderPtr->action7();
        }
    }
}

void Room::shaderAction8()
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->shaderPtr->action8();
        }
    }
}

void Room::synthAction(unsigned char input_data[])
{
    for (spriteIterator = spriteMap.begin(); spriteIterator != spriteMap.end(); spriteIterator++)
    {
        if ((*spriteIterator).second->hasShader)
        {
            (*spriteIterator).second->shaderPtr->synthAction(input_data);
        }
    }
}

