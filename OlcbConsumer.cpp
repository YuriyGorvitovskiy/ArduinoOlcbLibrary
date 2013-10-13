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
 
#include "Arduino.h"
#include "OlcbEvent.h"
#include "OlcbLink.h"

OlcbConsumer::OlcbConsumer() :
	link(NULL),
	next(NULL) {
}
	
void OlcbConsumer::addConsumer(OlcbConsumer* consumer) {
	if (next == NULL)
		next = consumer;
	else
		next->addConsumer(consumer);
}

void OlcbConsumer::handleReady(boolean ready) {
	onReady(ready);
		
	if (next != NULL)
		next->handleReady(ready);
}

void OlcbConsumer::handleEvent(OlcbEvent& event, OlcbConsumer* ignore) {
	if (ignore != this)
		onEvent(event);
		
	if (next != NULL)
		next->handleEvent(event, ignore);
}

void OlcbConsumer::onReady(boolean) {
}

void OlcbConsumer::onEvent(OlcbEvent& event) {
}

void OlcbConsumer::begin(OlcbLink& aLink) {
	link = &aLink;
	addConsumer(this);
}
