#include <avr/interrupt.h>
#include <SPI.h>
#include <UMessage.h>
#include <UMessageList.h>
#include <UMachine.h>
#include <UMachineDispatch.h>
#include <UMachineCronMessage.h>
#include <UMachineCron.h>
#include <UMachineMillisTicker.h>
#include <UCallbackMessage.h>
#include <UMachineCallback.h>
#include <USPIMessage.h>
#include <UMachineSPI.h>
#include <BBBeat.h>
#include <BBPinOut.h>
#include <BBSequencer.h>
#include <BBRootSequencer.h>
#include <BBPinSequencer.h>
#include <BBSerialSequencer.h>
#include <BBInterface.h>

/* Channels */

#define CHANNEL_COUNT 3

#define CHANNEL0 0
#define CHANNEL1 1
#define CHANNEL2 2

#define CHANNEL0_PIN 14
#define CHANNEL1_PIN 15
#define CHANNEL2_PIN 16

/* Beats */

#define BEAT_PERIOD 500
#define BEAT_COUNT 4

/* UI control pins */

#define UI_LATCH_PIN 6
#define UI_CLOCK_ENABLE_PIN 9

/* The time between sampling beats (ms) */

#define UI_IDLE_TIME 10

/* The time to wait for the 74HC165 to load after latching. (ms) */

#define UI_SAMPLE_TIME 3

/* Peroidic action scheduler*/
UMachineCron uCron = UMachineCron();

/* Hardware SPI */

UMachineSPI uSpi = UMachineSPI();

/* Send generate a 'tick' message every milisecond*/
UMachineMillisTicker uMillisTicker = UMachineMillisTicker(&uCron,&uCron.tick);

BBRootSequencer rootSequencer = BBRootSequencer(&uCron,BEAT_COUNT);
BBPinSequencer pinSequencer = BBPinSequencer(&uCron,&rootSequencer,CHANNEL_COUNT);
BBSerialSequencer serialSequencer = BBSerialSequencer(&rootSequencer,CHANNEL_COUNT);

BBInterface userInterface = BBInterface(&uCron,&uSpi,&rootSequencer,UI_IDLE_TIME,UI_SAMPLE_TIME);

UMachineCallback uCallbackDispatcher = UMachineCallback(&uCron);

void setup(){
  Serial.begin(9600);
  Serial.println("Starting");
  
  /* Initalize the spi for the UI. */
  
  uSpi.begin();
  
  /* Setup the chanel output pins */
  
  pinMode(CHANNEL0_PIN,OUTPUT);
  pinMode(CHANNEL1_PIN,OUTPUT);
  pinMode(CHANNEL2_PIN,OUTPUT);
  
  pinSequencer.setPin(CHANNEL0,CHANNEL0_PIN);
  pinSequencer.setPin(CHANNEL1,CHANNEL1_PIN);
  pinSequencer.setPin(CHANNEL2,CHANNEL2_PIN);
  
  /* Setup the delay between the beat message and raising
   * the output pin, desiged to effect latency compensation.
   */
  
  pinSequencer.setLatency(CHANNEL0,1);
  pinSequencer.setLatency(CHANNEL1,1);
  pinSequencer.setLatency(CHANNEL2,1);
  
  /*
   * Set the delay between beats
   */
  
  rootSequencer.setPeriod(BEAT_PERIOD);
  rootSequencer.addSequencer(&pinSequencer);
  rootSequencer.addSequencer(&serialSequencer);
  rootSequencer.setInterface(&userInterface);
  
  /* Initalize the ball baring interface */
  
  userInterface.init(UI_LATCH_PIN,UI_CLOCK_ENABLE_PIN);
    
  /* Start the sequencer */
    
  rootSequencer.send(&rootSequencer.beat,&uCron);
  uMillisTicker.send(&uMillisTicker.loop,&uMillisTicker);
  
  Serial.println("Done");
}
