#ifndef _ROOMMANAGER_HPP_
#define _ROOMMANAGER_HPP_

#include "room.hpp"
#include <map>
#include <vector>
#include <memory>

class RoomManager
{
private:
    int id;
    std::vector<std::shared_ptr<Room> > roomVector;
    std::vector<std::shared_ptr<Room> >::iterator roomIter;
    int activeRoom;
    std::shared_ptr<Room> arenaRoom;  // The current, playable arena
public:
    RoomManager();
    ~RoomManager();
    bool init();
    std::shared_ptr<Room> getRoom(std::string n);
    int getRoomIndex(std::string);
    void addRoom(std::shared_ptr<Room> r);
    void nextRoom();
    void changeRoom(std::string name);
    std::shared_ptr<Room> giveCurrentRoom();
    std::shared_ptr<Room> giveArenaRoom() { return arenaRoom; }
    void work();
};

#endif
