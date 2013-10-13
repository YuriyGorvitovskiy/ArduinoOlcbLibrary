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
 
#ifndef __OLCB_LINK_H__
#define __OLCB_LINK_H__

#include "OlcbNodeId.h"
#include "OlcbEvent.h"
#include "OlcbConsumer.h"


//OlcbEvent is usable till next call
typedef void (*OnEventFuncton)(OlcbEvent&);
typedef void (*OnReadyFuncton)(boolean);

class OlcbLink {
friend class OlcbConsumer;

protected:
	OlcbConsumer*		consumer;
	OnReadyFuncton		forReady;
	OnEventFuncton		forEvent;

	OlcbNodeId*			nodeId;
	
	OlcbEvent			rxEvent;
	OlcbEvent			txEvent;
	
	boolean				debugEnabled;
	 
protected:
						OlcbLink(boolean debug);
			void 		addConsumer(OlcbConsumer* consumer);

	virtual boolean		doPost(OlcbEvent& event, unsigned long waitMillis) = 0;

			boolean 	transmit(OlcbEvent& event, unsigned long waitMillis, void* consumer);
			void 		handleEvent(OlcbEvent& event, void* ignore);
			void 		handleReady(boolean ready);

			void		DEBUG(const char* msg);

public:	

			void 		begin(OlcbNodeId& nodeId);
			
	virtual boolean 	isReady() = 0;
			void 		subscribe(OnReadyFuncton onReady);
			void 		subscribe(OnEventFuncton onEvent);
			
			OlcbNodeId*	getNodeId();
			
	//CanMessage is usable till next call
	virtual OlcbEvent&  event(unsigned short mti, byte* data);
	
	//CanMessage is usable till next call
	//in case of wrong format return NULL
	virtual OlcbEvent*  txtEvent(const char* string);
	
	// Post events to network
			boolean 	post(OlcbEvent& event);							 // same as waitMillis = OLCB_NO_WAIT
			boolean 	post(OlcbEvent& event, unsigned long waitMillis); // waitMillis: timeout(ms), OLCB_NO_WAIT, OLCB_FOREVER
};

inline boolean OlcbLink::post(OlcbEvent& event)  {
	return transmit(event, OLCB_NO_WAIT, this);
}

inline boolean OlcbLink::post(OlcbEvent& event, unsigned long waitMillis) {
	return transmit(event, waitMillis, this);
}

inline boolean OlcbConsumer::post(OlcbEvent& event) {
	return link->transmit(event, OLCB_NO_WAIT, this);
}

inline boolean OlcbConsumer::post(OlcbEvent& event, unsigned long waitMillis) {
	return link->transmit(event, waitMillis, this);
}

inline void	OlcbLink::DEBUG(const char* msg) {
	if (debugEnabled) 
		Serial.println(msg);
}


#endif // __OLCB_LINK_H__