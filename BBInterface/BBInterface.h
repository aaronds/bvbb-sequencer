/**
 * BBInterface
 *
 */

#ifndef __BB_INTERFACE

#define __BB_INTERFACE

#include <UMachine.h>
#include <UMachineCron.h>
#include <UMachineSPI.h>
#include <BBRootSequencer.h>
#include <BBBeat.h>

#define BB_INTERFACE_BUFFER_SIZE 1

enum BBInterfaceState {BBInterfaceWaiting, BBInterfaceSampling, BBInterfaceLoading, BBInterfaceReading,BBInterfaceWaitingForSequencer, BBInterfacePause, BBInterfaceAdvance};

class BBInterface : public UMachine {

	public:
		BBInterface(UMachineCron *cron,UMachineSPI *spi,BBRootSequencer *seq,uint16_t it,uint16_t st);
		BBRootSequencer *sequencer;
		
		UMachineCron *uCron;
		UMachineCronMessage tick;

		UMachineSPI *uSpi;
		USPIMessage spiData;
		uint8_t spiBuffer[BB_INTERFACE_BUFFER_SIZE];

		/* The current beat. */
		uint8_t beatIdx;
		
		/* The time to wait between samples */
		uint16_t idleTime;

		/* Sample Time */
		uint8_t sampleTime;

		/* Latch pin to triger 74....*/
		uint8_t latchPin;

		/* The current state */
		BBInterfaceState state;

		void init(uint8_t lPin,uint8_t cePin);

		UMessage *receive(UMessage *msg);
};

#endif
