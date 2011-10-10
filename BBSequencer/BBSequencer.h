/**
 * BBSeqencer
 *
 * A super class for all sequencers.
 */

#ifndef __BB_SEQUENCER

#define __BB_SEQUENCER

#define BEAT_COUNT_MAX 8

#define BEAT_PULSE_LENGTH 50

#include <UMachine.h>
#include <UMachineCron.h>
#include <BBBeat.h>

class BBRootSequencer;

extern "C" { 
	#include <inttypes.h>
}

class BBSequencer : public UMachine {

	public:
		BBSequencer(BBRootSequencer *r,uint8_t cCount);
		UMessage *receive(UMessage *msg);

		BBRootSequencer *root;
		UMachine *next;
		uint8_t channelCount;
};

#endif
