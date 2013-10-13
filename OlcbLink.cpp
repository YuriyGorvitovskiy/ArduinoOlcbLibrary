/**
 ** This is Public Domain Software.
 ** 
 ** The author disclaims copyright to this source code.  
 ** In place of a legal notice, here is a blessing:
 **
 **    May you do good and not evil.
 **    May you find forgiveness for yourself and forgive others.
 **    May you share freely, never taking more than you give.
 **/
 
#include <Arduino.h>
#include <String.h>
#include "CanUtil.h"
#include "OlcbLink.h"
#include "OlcbCanLink.h"

OlcbLink::OlcbLink(boolean debug) :
	consumer(NULL),
	forReady(NULL),
	forEvent(NULL),
	nodeId(NULL),
	debugEnabled(debug) {
}

void OlcbLink::begin(OlcbNodeId& aNodeId) {
	nodeId = &aNodeId;
}

void OlcbLink::addConsumer(OlcbConsumer* aConsumer) {
	if (consumer == NULL)
		consumer = aConsumer;
	else
		consumer->addConsumer(aConsumer);
}

boolean OlcbLink::transmit(OlcbEvent& event, unsigned long waitMillis, void* consumer) {
	if (&event == NULL)
		return false;
		
	event.setSource(nodeId);
	if (!doPost(event, waitMillis))
		return false;
		
	handleEvent(event, consumer);
	return true;
}

void OlcbLink::handleReady(boolean ready) {
	if (forReady != NULL)
		forReady(ready);
		
	if (consumer != NULL)
		consumer->handleReady(ready);
}

void OlcbLink::handleEvent(OlcbEvent& event, void* ignore) {
	if (ignore != this && forEvent != NULL)
		forEvent(event);
		
	if (consumer != NULL)
		consumer->handleEvent(event, (OlcbConsumer*)ignore);
}

void OlcbLink::subscribe(OnReadyFuncton onReady) {
	forReady = onReady;
}

void OlcbLink::subscribe(OnEventFuncton onEvent) {
	forEvent = onEvent;
}

OlcbEvent& OlcbLink::event(unsigned short mti, byte* data) {
	txEvent.set(mti, data);
	return txEvent;
}

OlcbEvent* OlcbLink::txtEvent(const char* str) {
	unsigned short mti;
	byte  data[8];
	
	str++;
	switch(*str++) {
		case ':':
			mti  = (parseHexDigit(*str++)) << 12;
			mti |= (parseHexDigit(*str++)) << 8;
			mti |= (parseHexDigit(*str++)) << 4;
			mti |= (parseHexDigit(*str++));
			break;
		case 'R': // Report Event
			mti = OLCB_MIT_PCER;
			break;
		case 'T': // teach Event
			mti = OLCB_MIT_PTE;
			break;
		default:
			return NULL;
	}
			
	for(int i = 0; i < 8; ++i) {
		str++;
		data[i]  = (parseHexDigit(*str++)) << 4;
		data[i] |=  parseHexDigit(*str++);
	}
	return &event(mti, data);
}



