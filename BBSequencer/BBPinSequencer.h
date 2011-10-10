/**
 * BBPinSequencer
 *
 * A sequencer for output pins.
 */

#ifndef __BB_PIN_SEQUENCER
#define __BB_PIN_SEQUENCER

#include <UMachine.h>
#include <BBSequencer.h>
#include <BBPinOut.h>
#include <BBRootSequencer.h>

class BBPinSequencer : public BBSequencer {

	public:
		BBPinSequencer(UMachineCron *cron,BBRootSequencer *r,uint8_t cCount);
		UMessage *receive(UMessage *msg);

		/* Accessor methods */

		void setLatency(uint8_t beat,uint16_t latency);
		uint16_t getLatency(uint8_t beat);

		void setPin(uint8_t channel,uint8_t pin);
		uint8_t getPin(uint8_t channel);

		UMachineCron *uCron;
		BBPinOut beatUp[BEAT_COUNT_MAX];
		BBPinOut beatDown[BEAT_COUNT_MAX];
};
#endif
