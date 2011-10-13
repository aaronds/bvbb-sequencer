/**
 * BBMidiSequencer.
 */

#include "BBMidiSequencer.h"
#include <HardwareSerial.h>

BBMidiSequencer::BBMidiSequencer(UMachineCron *cron,uint16_t tpb,BBRootSequencer *root,uint8_t cCount) : BBSequencer(root,cCount){
	uCron = cron;
	ticksPerBeat = tpb;
	tickCount = 0;
}

UMessage *BBMidiSequencer::receive(UMessage *msg){

	if(msg == &root->actionBeat){
		if(state == BBMidiClockOff){
			Serial.write(0xF8);
			tick.repeat = root->getPeriod() / ticksPerBeat;
			tick.time = root->actionBeat.data; 
			send(&tick,uCron);
			state = BBMidiClockOn;
		}
		tickCount = 0;

		return BBSequencer::receive(msg);
	}

	if(msg == &tick){
		Serial.write(0xF8);

		if(tickCount > ticksPerBeat + 1){
			Serial.write(0xFC);
			state = BBMidiClockOff;
			return NULL;
		}

		tickCount++;

		return msg;
	}

	return BBSequencer::receive(msg);
}
