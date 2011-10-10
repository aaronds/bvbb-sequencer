#include "BBSerialSequencer.h"
#include <HardwareSerial.h>

BBSerialSequencer::BBSerialSequencer(BBRootSequencer *rs,uint8_t cCount) : BBSequencer(rs,cCount) {
	channelMask = 0;

	for(uint8_t i = 0;i < cCount;i++){
		channelMask += (1 << i);
	}
}

UMessage *BBSerialSequencer::receive(UMessage *msg){
	uint16_t serialMessage = 0;
	if(msg == &root->actionBeat){
		/* Make Noise the most significant bit determins action / update */
		serialMessage = 0x800 + (root->actionBeat.index << 8) | (root->actionBeat.data & 0xff);
		Serial.println(serialMessage,HEX);
	}else if(msg == &root->updateBeat){
		serialMessage = ((root->updateBeat.index & 0x7f) << 8) | (root->updateBeat.data & channelMask);
		Serial.println(serialMessage,HEX);
	}

	return BBSequencer::receive(msg);
}
