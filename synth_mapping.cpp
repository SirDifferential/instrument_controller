#include "synth_mapping.hpp"
#include "toolbox.hpp"
#include "manager.hpp"

#include <vector>
#include <algorithm>

/**
 * My synth has the following system:
 * 
 * MIDI output has 4 different bytes of data
 *
 * First byte: Command byte. 144 appears to be some kind of key state change
 * Second byte: Key involved. Usually a note
 * Third byte: Key up or down
 * Fourth byte: ???
 *
 * 144 40 64 3 == Note 40 pressed down
 * 144 40 0 4 == Note 40 pulled up
 * 123 0 0 2 == No keys pressed
 * 
 */


SynthMapping::SynthMapping()
{
	key_states = tbox.giveIntArray2D(4, 220);
	downKeys = tbox.giveIntArray1D(220);
	downTimes = tbox.giveIntArray1D(220);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 220; j++)
		{
			key_states[i][j] = 0;
			downKeys[j] = 0;
			downTimes[j] = 0;
		}
	}
	downCount = 0;
}

SynthMapping::~SynthMapping()
{
	tbox.deleteIntArray2D(key_states, 4);
	tbox.deleteIntArray1D(downKeys);
	tbox.deleteIntArray1D(downTimes);
}

bool SynthMapping::isKeyDown(int key)
{
	if (key_states[2][key] == 64)
		return true;
	else
		return false;
}

void SynthMapping::pushKeyDown(int key)
{
	fprintf(stderr, "Pushing %d down\n",key);
	key_states[2][key] = 64;
	downKeys[key] = 1;
	downTimes[key] = int(manager.getOpengl()->giveTicks());
	downCount++;
}

void SynthMapping::pullKeyUp(int key)
{
	fprintf(stderr, "Pulling %d up\n", key);
	key_states[2][key] = 0;
	downKeys[key] = 0;
	downTimes[key] = int(manager.getOpengl()->giveTicks());
	downCount--;
	if (downCount <= 0)
	{
		fprintf(stderr, "all keys up\n");
		downCount = 0;
		//allKeysUp();
	}
}

int** SynthMapping::getKeyStates()
{
	return key_states;
}

void SynthMapping::allKeysUp()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 220; j++)
		{
			key_states[i][j] = 0;
			downKeys[j] = 0;
			downTimes[j] = 0;
		}
	}
}

int* SynthMapping::getDownKeys()
{
	return downKeys;
}

int* SynthMapping::getDownTimes()
{
	return downTimes;
}

