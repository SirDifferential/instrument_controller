#include "inputReader.hpp"
#include "manager.hpp"
#include "inputmanager.hpp"

#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

int InputReader::init()
{
	fprintf(stderr, "Initting SFD\n");
	device_name = "/dev/snd/midiC1D0";
	sequencer_file_descriptor = open(device_name.c_str(), O_RDONLY);
	if (sequencer_file_descriptor == -1)
	{
		fprintf(stderr, "!!!! Error opening sequencer file descriptor !!!!\n");
		return 11;
	}

	fcntl(sequencer_file_descriptor, F_SETFL, O_NONBLOCK);

	fprintf(stderr, "\n****************************************\n");
	fprintf(stderr, "* InputReader started successfully!!!! *\n");
	fprintf(stderr, "****************************************\n\n");
	return 0;
}

int InputReader::readInput()
{
	status = read(sequencer_file_descriptor, &input_bytes, sizeof(input_bytes));
	if (status < 0)
	{
		//fprintf(stderr, "Error reading input\n");
		//fprintf(stderr, device_name.c_str());
		return -12;
	}

	if (input_bytes[0] && (input_bytes[0] == 144 ))
	{
	//	fprintf(stderr, "MIDI Event: %d %d %d %d\n", input_bytes[0], input_bytes[1], input_bytes[2], input_bytes[4]);
		manager.getInputMgr()->handleSynthEvents(input_bytes);
	}
	return 0;
}

InputReader::InputReader()
{
	fprintf(stderr, "InputReader warming up\n");
	init();
}

InputReader::~InputReader()
{
}

