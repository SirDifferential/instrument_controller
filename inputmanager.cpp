#include "inputmanager.hpp"
#include "renderer.hpp"
#include "manager.hpp"
#include "roommanager.hpp"
#include "room.hpp"
#ifdef WIN32
#include <SDL.h>
#include <windows.h>
#else
#include <SDL/SDL.h>
#endif
#include <memory>

Inputmanager::Inputmanager()
{
    fprintf(stderr, "Inputmanager being hired...\n");
	synthMappings = std::shared_ptr<SynthMapping>(new SynthMapping());
}

Inputmanager::~Inputmanager()
{
    fprintf(stderr, "Inputmanager being fired...\n");
}

// Resets all keys as pressable at the beginning of the loop
// TODO: Make into a keystate array
void Inputmanager::resetKeys()
{
    triggeredLeft = false;
    triggeredRight = false;
    triggeredUp = false;
    triggeredDown = false;
    triggeredA = false;
    triggeredD = false;
    triggeredZ = false;
    triggeredX = false;
    triggeredK = false;
}

void Inputmanager::checkInput()
{
   std::shared_ptr<Room> currentRoom = manager.getRoomMgr()->giveCurrentRoom();
    if (currentRoom->getType() == 3 || currentRoom->getType() == 4)  // cinematics / text
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    manager.stop();
                    break;

                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            currentRoom->iterateSprites();
                            break;
                        case SDLK_RETURN:
                            currentRoom->iterateSprites();
                            break;
                    }
                    break;
            }
        }
    } else
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    manager.stop();
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            manager.stop();
                            break;
                        case SDLK_a:
                            currentRoom->toggleShaders();
                            break;
                        case SDLK_q:
                            currentRoom->shaderAction1();
                            break;
                        case SDLK_w:
                            currentRoom->shaderAction2();
                            break;
                        case SDLK_e:
                            currentRoom->shaderAction3();
                            break;
                        case SDLK_r:
                            currentRoom->shaderAction4();
                            break;
                        case SDLK_t:
                            currentRoom->shaderAction5();
                            break;
                        case SDLK_y:
                            currentRoom->shaderAction6();
                            break;
                        case SDLK_u:
                            currentRoom->shaderAction7();
                            break;
                        case SDLK_i:
                            currentRoom->shaderAction8();
                            break;
                        case SDLK_UP:
                            renderer.moveForwards();
                            break;
                        case SDLK_DOWN:
                            renderer.moveBackwards();
                            break;
                        case SDLK_LEFT:
                            renderer.moveLeft();
                            break;
                        case SDLK_RIGHT:
                            renderer.moveRight();
                            break;
                        case SDLK_h:
                            renderer.panUp();
                            break;
                        case SDLK_j:
                            renderer.panDown();
                            break;
                        case SDLK_n:
                            renderer.panLeft();
                            break;
                        case SDLK_m:
                            renderer.panRight();
                            break;
                    }
                break;
            }
        }
    }
}

void Inputmanager::handleSynthEvents(unsigned char input_data[])
{
	fprintf(stderr, "%d %d %d %d\n", input_data[0], input_data[1], input_data[2], input_data[3]);
	if (input_data[0] == 144)
	{
		if (input_data[2] == 64)
		{
			synthMappings->pushKeyDown(input_data[1]);
		}
		else if (input_data[2] == 0)
		{
			synthMappings->pullKeyUp(input_data[1]);
		}
	}
	else if (input_data[0] == 123)
	{
		fprintf(stderr, "All keys up\n");
		synthMappings->allKeysUp();
	}
	else
	{
		fprintf(stderr, "fail safe\n");
		synthMappings->allKeysUp();
	}
	manager.getRoomMgr()->giveCurrentRoom()->synthAction(input_data);

}

int* Inputmanager::getSynthKeysDown()
{
	return synthMappings->getDownKeys();
}

int* Inputmanager::getSynthDownTimes()
{
	return synthMappings->getDownTimes();
}

