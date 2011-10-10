/**
 * BBRootSequencer
 *
 * A sequencer to recieve beat state messages
 */

#ifndef __BB_ROOT_SEQUENCER
#define __BB_ROOT_SEQUENCER

#include <UMachine.h>
#include <BBBeat.h>

#include <BBSequencer.h>

class BBRootSequencer : public UMachine {

	public:
		BBRootSequencer(UMachineCron *cron,uint8_t bCount);
		UMessage *receive(UMessage *msg);

		void setPeriod(uint16_t period);
		uint16_t getPeriod();
		
		void setBeatDelay(uint16_t delay);
		uint16_t getBeatDelay();

		void setInterface(UMachine *interface);
		UMachine *getInterface();

		void addSequencer(BBSequencer *seq);

		uint8_t beatState[BEAT_COUNT_MAX];
		UMachineCronMessage beat;
		UMachineCron *uCron;
		UMachine *interface;

		/* List of sequencers to pass messages to */

		BBSequencer *start;
		BBSequencer *end;

		BBBeat updateBeat; //State from whoever is providing the interface.
		BBBeat actionBeat; //State to be sent to child sequencers.

		uint8_t beatIndex;
		uint8_t beatCount;
};

#endif
