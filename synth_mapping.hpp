#ifndef __SYNTH_MAPPING_HPP_
#define __SYNTH_MAPPING_HPP_

class SynthMapping
{
private:
	int** key_states;
	int* downKeys;
	int* downTimes;
	int downCount;
public:
	SynthMapping();
	~SynthMapping();

	void updateMapping(unsigned char data[]);
	bool isKeyDown(int keyNumber);
	void pushKeyDown(int keyNumber);
	void pullKeyUp(int keyNumber);
	int** getKeyStates();
	void allKeysUp();
	int* getDownKeys();
	int* getDownTimes();
	void updateDownKeys();
};

#endif

