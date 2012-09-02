#ifndef _MANAGER_HPP_
#define _MANAGER_HPP_

#include <vector>
#include <memory>
#include "opengl.hpp"
#include "roommanager.hpp"
#include "inputmanager.hpp"
#include "inputReader.hpp"

class Manager
{
private:
    int id;
    int argc;
    char** argv;
    bool running;
    int nextFPS;
    float gameTime;
    bool debug;
    std::shared_ptr<Opengl> openglPtr;
    std::shared_ptr<RoomManager> roomManagerPtr;
	std::shared_ptr<Inputmanager> inputManagerPtr;
	std::shared_ptr<InputReader> inputReaderPtr;
public:
    Manager();
    ~Manager();
    int run();
    void stop();
    void setParams(int argc, char** args);
    double getTime() { return gameTime; }
    void getEnter();
    std::shared_ptr<Opengl> getOpengl() { return openglPtr; }
    std::shared_ptr<RoomManager> getRoomMgr() { return roomManagerPtr; }
	std::shared_ptr<Inputmanager> getInputMgr() { return inputManagerPtr; }
};

extern Manager manager;

#endif

