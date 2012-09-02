#ifndef _INPUTMANAGER_HPP_
#define _INPUTMANAGER_HPP_

#include "synth_mapping.hpp"
#include <memory>
class Menu;

class Inputmanager
{
private:
    int id;
    // These flags are used to limit key presses on Linux
    // Input is checked from multiple places, and on Linux
    // this causes multiple presses of same key per frame
    bool triggeredLeft;
    bool triggeredRight;
    bool triggeredUp;
    bool triggeredDown;
    bool triggeredA;
    bool triggeredD;
    bool triggeredZ;
    bool triggeredX;
    bool triggeredK;
	std::shared_ptr<SynthMapping> synthMappings;
public:
    Inputmanager();
    ~Inputmanager();
    
    void resetKeys();
    void checkInput();
	void handleSynthEvents(unsigned char input_data[]);
	int* getSynthKeysDown();
	int* getSynthDownTimes();
};

#endif

