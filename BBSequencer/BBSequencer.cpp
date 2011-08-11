#include <BBSequencer.h>
#include <WProgram.h>

BBSequencer::BBSequencer(UMachineCron *c,uint8_t cc,uint8_t bc){
	channelCount = cc;
	beatCount = bc;
	uCron = c;
}

UMessage *BBSequencer::receive(UMessage *msg){
	uint8_t i,mask = 1;
	
	if(msg == &state){
		if(beatStates[state.index] != state.state){
			beatStates[state.index] = state.state;
		}
		return NULL;
	}else if(msg == &beat){
		/*
		 * The Beat message
		 *
		 * Messages to cron
		 */

		for(i = 0;i < channelCount;i++){
			if(beatStates[beatIdx] & mask){
				send(&beatUp[i],uCron);
			}
			mask = mask << 1;
		}

		beatIdx = (beatIdx + 1) % beatCount;

		return &beat;
	}

	/*
	 * Messages back from cron, raise the pins
	 */

	for(i = 0;i < channelCount;i++){
		if(&beatUp[i] == msg){
			digitalWrite(beatUp[i].pin,HIGH);
			beatDown[i].time = BEAT_PULSE_LENGTH;
			send(&beatDown[i],uCron);
			return NULL;
		}
	}

	for(i = 0;i < channelCount;i++){
		if(&beatDown[i] == msg){
			digitalWrite(beatDown[i].pin,LOW);
			return NULL;
		}
	}
}


void BBSequencer::setDelay(uint8_t beat,uint16_t delay){
	beatUp[beat].repeat = delay;
}

uint16_t BBSequencer::getDelay(uint8_t beat){
	return beatUp[beat].repeat;
}

void BBSequencer::setPin(uint8_t channel,uint8_t pin){
	beatUp[channel].pin = pin;
	beatDown[channel].pin = pin;
}

uint8_t BBSequencer::getPin(uint8_t channel){
	return beatUp[channel].pin;
}

void BBSequencer::setPeriod(uint16_t period){
	beat.repeat = period;
}
