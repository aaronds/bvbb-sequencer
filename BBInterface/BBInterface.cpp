#include "BBInterface.h"
#include <WProgram.h>

BBInterface::BBInterface(UMachineCron *cron,BBSequencer *seq,uint16_t it,uint16_t st) : UMachine() {
	uCron = cron;
	sequencer = seq;
	beatIdx = 0;
	idleTime = it;
	sampleTime = st; 
	state = BBInterfaceWaiting;
}

void BBInterface::init(uint8_t lp,uint8_t cePin){
	tick.time = idleTime;
	send(&tick,uCron);

	latchPin = lp;
	clockEnablePin = cePin;

	pinMode(latchPin,OUTPUT);
	digitalWrite(latchPin,HIGH);

	pinMode(clockEnablePin,OUTPUT);
	digitalWrite(clockEnablePin,HIGH);
}

UMessage *BBInterface::receive(UMessage *msg){
	switch(state){
		/* Not doing anything
		 * Trasition the 74HC... latch pin high - low - high to load the state transfer.
		 */
		case BBInterfaceWaiting:
			digitalWrite(latchPin,LOW);
			tick.time = sampleTime;
			send(&tick,uCron);
			state = BBInterfaceSampling;
			break;

		/* Loading done, transiation to high. */
		case BBInterfaceSampling:
			digitalWrite(latchPin,HIGH);
			tick.time = sampleTime;
			send(&tick,uCron);
			state = BBInterfaceLoading;
			break;

		case BBInterfaceLoading:
			digitalWrite(clockEnablePin,LOW);
			SPDR = (1 << beatIdx);
			send(&tick,this);
			state = BBInterfaceReading;	
			break;

		case BBInterfaceReading:
			if(!(SPSR & _BV(SPIF))){ /* Loop Until */
				send(&tick,this); 
			}else{
				digitalWrite(clockEnablePin,HIGH);
				sequencer->state.state = ~SPDR;
				sequencer->state.index = (beatIdx + sequencer->beatCount - 1) % sequencer->beatCount;
				send(&sequencer->state,sequencer);
				beatIdx = ((beatIdx + 1) % sequencer->beatCount);
				tick.time = sampleTime;
				send(&tick,uCron);
				state = BBInterfacePause;
			}
			break;

		case BBInterfacePause:
			digitalWrite(clockEnablePin,LOW);
			SPDR = (1 << beatIdx);
			send(&tick,this);
			state = BBInterfaceAdvance;
			break;

		case BBInterfaceAdvance:
			if(!(SPSR & _BV(SPIF))){ /* Loop Until */
				send(&tick,this); 
			}else{
				tick.time = idleTime;
				send(&tick,uCron);
				state = BBInterfaceWaiting;
			}
			break;

		default:
			/* VVV Wrong */
			state = BBInterfaceWaiting;
			tick.time = idleTime;
			send(&tick,uCron);
			break;

	}
	return NULL;
}
