#include "BBInterface.h"
#include <WProgram.h>
#include <HardwareSerial.h>

BBInterface::BBInterface(UMachineCron *cron,UMachineSPI *spi,BBRootSequencer *seq,uint16_t it,uint16_t st) : UMachine() {
	uCron = cron;
	sequencer = seq;
	beatIdx = 0;
	idleTime = it;
	sampleTime = st; 
	state = BBInterfaceWaiting;
	
	uSpi = spi;
	spiData.buffer = spiBuffer;
	spiData.dataMode = SPI_MODE0;
	spiData.bitOrder = MSBFIRST;
	spiData.length = BB_INTERFACE_BUFFER_SIZE;
}

void BBInterface::init(uint8_t lp,uint8_t cePin){
	tick.time = idleTime;
	send(&tick,uCron);

	latchPin = lp;
	spiData.clockEnablePin = cePin;

	pinMode(latchPin,OUTPUT);
	digitalWrite(latchPin,HIGH);

	pinMode(spiData.clockEnablePin,OUTPUT);
	digitalWrite(spiData.clockEnablePin,HIGH);
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
			*((uint8_t *) spiData.buffer) = (1 << beatIdx);
			send(&spiData,uSpi);
			state = BBInterfaceReading;	
			break;

		case BBInterfaceReading:
			sequencer->updateBeat.data = (uint8_t) ~(*((uint8_t *) spiData.buffer));
			sequencer->updateBeat.index = (beatIdx) % sequencer->beatCount;
			send(&sequencer->updateBeat,sequencer);
			beatIdx = ((beatIdx + 1) % sequencer->beatCount);
			state = BBInterfaceWaitingForSequencer;
			break;

		case BBInterfaceWaitingForSequencer:
			tick.time = sampleTime;
			send(&tick,uCron);
			state = BBInterfacePause;
			break;

		case BBInterfacePause:
			*((uint8_t *) spiData.buffer) = (1 << beatIdx);
			send(&spiData,uSpi);
			state = BBInterfaceAdvance;
			break;

		case BBInterfaceAdvance:
			tick.time = idleTime;
			send(&tick,uCron);
			state = BBInterfaceWaiting;
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
