/**
 * BBBeat
 *
 * A message to represent a beat can be either a change or an action.
 */

#ifndef __BB_BEAT
#define __BB_BEAT

#include <UMessage.h>

extern "C" { 
	#include <inttypes.h>
}

class BBBeat : public UMessage {

	public:
		BBBeat();
		uint8_t index;
		uint16_t data;
};

#endif
