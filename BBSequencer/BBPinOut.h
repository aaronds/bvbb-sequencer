/**
 * BBPinOut
 *
 * A representation of an output pin.
 */

#ifndef __BB_PIN_OUT
#define __BB_PIN_OUT

#include <UMachineCronMessage.h>

extern "C" { 
	#include <inttypes.h>
}

class BBPinOut : public UMachineCronMessage {

	public:
		BBPinOut();
		uint8_t pin;
};

#endif
