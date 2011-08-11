/**
 * BBInterface
 *
 */

#ifndef __BB_INTERFACE

#define __BB_INTERFACE

#include <UMachine.h>
#include <UMachineCron.h>
#include <BBSequencer.h>
#include <BBBeatState.h>

enum BBInterfaceState {BBInterfaceWaiting, BBInterfaceSampling, BBInterfaceLoading, BBInterfaceReading, BBInterfacePause, BBInterfaceAdvance};

class BBInterface : public UMachine {

	public:
		BBInterface(UMachineCron *cron,BBSequencer *seq,uint16_t it,uint16_t st);
		BBSequencer *sequencer;
		UMachineCron *uCron;
		UMachineCronMessage tick;

		/* The current beat. */
		uint8_t beatIdx;
		
		/* The time to wait between samples */
		uint16_t idleTime;

		/* Sample Time */
		uint8_t sampleTime;

		/* Latch pin to triger 74....*/
		uint8_t latchPin;

		/* Clock Enable Pin */
		uint8_t clockEnablePin;

		/* The current state */
		BBInterfaceState state;

		void init(uint8_t lPin,uint8_t cePin);

		UMessage *receive(UMessage *msg);
};

#endif __BB_INTERFACE
