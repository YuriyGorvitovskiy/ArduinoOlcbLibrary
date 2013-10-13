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
 
#ifndef __OLCB_CONSUMER_H__
#define __OLCB_CONSUMER_H__

#include "Can.h"
#include "OlcbEvent.h"

class OlcbLink;

#define OLCB_NO_WAIT (CAN_NO_WAIT)
#define OLCB_FOREVER (CAN_FOREVER)

class OlcbConsumer {
	friend class 	OlcbLink;
	
protected:
	
	OlcbLink*		link;	
	OlcbConsumer*  	next;
	
					OlcbConsumer();

			void	addConsumer(OlcbConsumer* consumer);
			void 	handleReady(boolean ready);
			void 	handleEvent(OlcbEvent& event, OlcbConsumer* ignore);
	
	//CanEvent is usable till end of the call
	virtual void 	onReady(boolean ready); // message handling is optional
	virtual void 	onEvent(OlcbEvent& event); // message handling is optional

			//implementation of this functions is in OlcbLink.h	
			boolean post(OlcbEvent& event);							 // same as waitMillis = OLCB_NO_WAIT
			boolean post(OlcbEvent& event, unsigned long waitMillis); // waitMillis: timeout(ms), OLCB_NO_WAIT, OLCB_FOREVER

public:

	//Derived class can override this method to do additional initialization but should call OlcbConsumer::begin(...) 
	virtual void	begin(OlcbLink& link); 
};

#endif 