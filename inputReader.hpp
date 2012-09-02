#ifndef __INPUTREADER_HPP_
#define __INPUTREADER_HPP_

#include <string>

class InputReader
{
private:
	int sequencer_file_descriptor;
	std::string device_name;
	unsigned char input_bytes[4];
	int status;
public:
	InputReader();
	~InputReader();

	int init();
	int readInput();
};

#endif

