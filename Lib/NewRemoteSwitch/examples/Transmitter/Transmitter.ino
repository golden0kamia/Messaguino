/*
 * Demo for RF remote switch transmitter.
 * For details, see NewRemoteTransmitter.h!
 *
 * Connect the transmitter to digital pin 11.
 */
 
#include <NewRemoteTransmitter.h>

unsigned long int adress = 13709258;
int sendPin = 11
int unit = 0

void setup() {
	NewRemoteTransmitter transmitter(adress, sendPin);
}

void loop() {
    transmitter.sendGroupDim(receivedCode.dimLevel);
    transmitter.sendDim(receivedCode.unit, receivedCode.dimLevel);
    transmitter.sendGroup(isOn);
    transmitter.sendUnit(unit, isOn);
}

