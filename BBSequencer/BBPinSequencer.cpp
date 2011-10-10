#include <BBPinSequencer.h>
#include <WProgram.h>

BBPinSequencer::BBPinSequencer(UMachineCron *c,BBRootSequencer *rs,uint8_t cc) : BBSequencer(rs,cc) {
	uCron = c;
}

UMessage *BBPinSequencer::receive(UMessage *msg){
	uint8_t mask = 1;
	uint8_t i = 0;

	if(msg == &root->actionBeat){
		for(i = 0;i < channelCount;i++){
			if(root->beatState[root->actionBeat.index] & mask){
				/* For the action beat the data is the no of 
				 * milliseconds till the real beat
				 */
				if(root->actionBeat.data > beatUp[i].repeat){
					/*Figure out when to send the signal to compensate
					 *for latency.
					 */

					beatUp[i].time = root->actionBeat.data - beatUp[i].repeat;
					send(&beatUp[i],uCron);
				}else{
					send(&beatUp[i],this);
				}
			}
			mask = mask << 1;
		}
		return BBSequencer::receive(msg);
	}

	if(msg == &root->updateBeat){
		/* We use the root sequencers beat state */
		return BBSequencer::receive(msg);
	}

	for(i = 0;i < channelCount;i++){
		if(&beatUp[i] == msg){
			digitalWrite(beatUp[i].pin,HIGH);
			beatDown[i].time = BEAT_PULSE_LENGTH;
			send(&beatDown[i],uCron);
			return NULL;
		}else if(&beatDown[i] == msg){
			digitalWrite(beatDown[i].pin,LOW);
			return NULL;
		}
	}
	return NULL;
}

void BBPinSequencer::setLatency(uint8_t beat,uint16_t delay){
	beatUp[beat].repeat = delay;
}

uint16_t BBPinSequencer::getLatency(uint8_t beat){
	return beatUp[beat].repeat;
}

void BBPinSequencer::setPin(uint8_t channel,uint8_t pin){
	beatUp[channel].pin = pin;
	beatDown[channel].pin = pin;
}

uint8_t BBPinSequencer::getPin(uint8_t channel){
	return beatUp[channel].pin;
}
