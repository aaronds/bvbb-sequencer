/**
 * BBSeqencer
 *
 * Schedue the pins.
 */

#ifndef __BB_SEQUENCER

#define __BB_SEQUENCER

#define BEAT_COUNT_MAX 8

#define BEAT_PULSE_LENGTH 50

#include <UMachine.h>
#include <UMachineCron.h>
#include <BBPinOut.h>
#include <BBBeatState.h>

extern "C" { 
	#include <inttypes.h>
}


class BBSequencer : public UMachine {

	public:

		BBPinOut beatUp[BEAT_COUNT_MAX];
		BBPinOut beatDown[BEAT_COUNT_MAX];
		UMachineCronMessage beat;
		UMachineCron *uCron;

		/* The message to be recived from the ui. */

		BBBeatState state;

		/* Sequencer config */

		uint8_t channelCount; 
		uint8_t beatCount;

		/*Machine state variables*/

		uint8_t beatStates[BEAT_COUNT_MAX];
		uint8_t beatIdx;
	
		BBSequencer(UMachineCron *cron,uint8_t cc,uint8_t bc);

		UMessage *receive(UMessage *msg);

		/* Accessor methods */

		void setDelay(uint8_t beat,uint16_t latency);
		uint16_t getDelay(uint8_t beat);

		void setPin(uint8_t channel,uint8_t pin);
		uint8_t getPin(uint8_t channel);

		void setPeriod(uint16_t period);
		uint16_t getPeriod();
};

#endif
