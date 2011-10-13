#include <BBSPISequencer.h>

BBSPISequencer::BBSPISequencer(UMachineSPI *spi,uint8_t cep,BBRootSequencer *r,uint8_t cCount) : BBSequencer(r,cCount){
	uSpi = spi;
	spiBeat.clockEnablePin = cep;
	spiBeat.dataMode = SPI_MODE0;
	spiBeat.bitOrder = MSBFIRST;
	spiBeat.length = sizeof(buffer);
	spiBeat.buffer = buffer;
}

UMessage *BBSPISequencer::receive(UMessage *msg){
	
	/* Return of spi message */
	if(msg == &spiBeat){
		return NULL;
	}

	/* Beat */
	if(msg == &root->actionBeat){
		buffer[0] = root->actionBeat.data & 0xff;
		send(&spiBeat,uSpi);
	}

	return BBSequencer::receive(msg); 
}
