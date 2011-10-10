#include <BBSequencer.h>
#include <WProgram.h>

BBSequencer::BBSequencer(BBRootSequencer *r,uint8_t cc){
	root = r;
	channelCount = cc;
	next = NULL;
}

UMessage *BBSequencer::receive(UMessage *msg){
	if(next){
		send(msg,next);
	}
	return NULL;
}
