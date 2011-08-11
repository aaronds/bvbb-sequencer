/**
 * BBBeatState
 *
 * A message to represent a beat state.
 *
 * 
 */

#ifndef __BB_BEAT_STATE
#define __BB_BEAT_STATE

#include <UMessage.h>

extern "C" { 
	#include <inttypes.h>
}

class BBBeatState : public UMessage {

	public:
		BBBeatState();
		uint8_t index;
		uint8_t state;

};
#endif
