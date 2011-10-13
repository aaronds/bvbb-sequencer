/**
 * BBMidiSequencer
 *
 */

#ifndef __BB_MIDI_SEQUENCER

#define __BB_MIDI_SEQUENCER

#include <UMachine.h>
#include <UMachineCron.h>
#include <UMachineCronMessage.h>
#include <BBSequencer.h>
#include <BBRootSequencer.h>

enum BBMidiSequencerState {BBMidiClockOff,BBMidiClockOn};

class BBMidiSequencer : public BBSequencer {

	public:
		BBMidiSequencer(UMachineCron *cron,uint16_t tpb,BBRootSequencer *r,uint8_t cCount);
		UMessage *receive(UMessage *msg);

		BBRootSequencer *sequencer;
		UMachineCron *uCron;
		UMachineCronMessage tick;
		BBMidiSequencerState state;
		uint16_t tickCount;
		uint16_t ticksPerBeat;
};
#endif
