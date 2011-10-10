/**
 * BBSerialSequencer
 *
 * Broadcast sequencer messages
 */

#ifndef __BB_SERIAL_SEQUENCER
#define __BB_SERIAL_SEQUENCER

#include <UMachine.h>
#include <BBSequencer.h>
#include <BBRootSequencer.h>

class BBSerialSequencer : public BBSequencer {

	public:
		BBSerialSequencer(BBRootSequencer *r,uint8_t cCount);
		UMessage *receive(UMessage *msg);
		uint8_t channelMask;
};

#endif
