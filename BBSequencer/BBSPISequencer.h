/**
 * BBSPISequencer
 *
 * Send state over spi.
 */

#ifndef __BB_SPI_SEQUENCER
#define __BB_SPI_SEQUENCER

#include <UMachine.h>
#include <UMachineCron.h>
#include <UMachineCronMessage.h>
#include <BBSequencer.h>
#include <BBRootSequencer.h>
#include <UMachineSPI.h>
#include <USPIMessage.h>

class BBSPISequencer : public BBSequencer {

	public:
		BBSPISequencer(UMachineSPI *spi,uint8_t cep,BBRootSequencer *r,uint8_t cCount);
		UMessage *receive(UMessage *msg);

		UMachineSPI *uSpi;
		USPIMessage spiBeat;
		uint8_t buffer[1];
};
#endif
