#include "BBRootSequencer.h"
#include <HardwareSerial.h>

BBRootSequencer::BBRootSequencer(UMachineCron *cron,uint8_t bCount){
	uint8_t i = 0;
	
	uCron = cron;
	beatCount = bCount;

	for(i = 0;i < BEAT_COUNT_MAX;i++){
		beatState[i] = 0;
	}

	updateBeat.data = 0;
	updateBeat.index = 0;

	actionBeat.data = 0;
	actionBeat.index = 0;

	start = NULL;
	end = NULL;

}

UMessage *BBRootSequencer::receive(UMessage *msg){
	if(msg == &updateBeat){
		if(msg->from == end){
			/* Return to interface */
			send(msg,interface);
		}else{
			if(beatState[updateBeat.index] != updateBeat.data){
				/* Beat Is Dirty */
				beatState[updateBeat.index] = (uint8_t) updateBeat.data;
				send(msg,start);
			}else{
				send(msg,interface);
			}
		}
	}

	if(msg == &actionBeat){
		/* Finished making noises */
		return NULL;
	}

	if(msg == &beat){
		/* Make some noise! */
		actionBeat.index = beatIndex;
		send(&actionBeat,start);
		beatIndex = (beatIndex + 1) % beatCount;
		return msg;
	}

	return NULL;
}

void BBRootSequencer::setPeriod(uint16_t period){
	beat.repeat = period;
}

uint16_t BBRootSequencer::getPeriod(){
	return beat.repeat;
}

void BBRootSequencer::addSequencer(BBSequencer *seq){	
	if(end == NULL){
		/* Nothing in the list*/
		start = seq;
	}else{
		end->next = seq;
	}	

	end = seq;
	end->next = this;
}

void BBRootSequencer::setInterface(UMachine *intf){
	interface = intf;
}

UMachine *BBRootSequencer::getInterface(){
	return interface;
}

void BBRootSequencer::setBeatDelay(uint16_t delay){
	actionBeat.data = delay;
}

uint16_t BBRootSequencer::getBeatDelay(){
	return actionBeat.data;
}
