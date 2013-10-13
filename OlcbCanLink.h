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
 
#ifndef __OLCB_CAN_LINK_H__
#define __OLCB_CAN_LINK_H__

#include "Can.h"
#include "OlcbLink.h"

enum OlcbCanLinkState {
	OlcbCanLinkInhibited,
	OlcbCanLinkInhibited_CID7_Sent,
	OlcbCanLinkInhibited_CID6_Sent,
	OlcbCanLinkInhibited_CID5_Sent,
	OlcbCanLinkInhibited_CID4_Sent,
	OlcbCanLinkInhibited_RID_Sent,
	OlcbCanLinkPermitted,
	OlcbCanLinkUnusable
};

class OlcbCanLink;

class OlcbCanLink : public OlcbLink, protected CanConsumer {
protected:
	static  OlcbCanLink* 		nodeIdResolver;
			short				aliasGenerator[4];
	
			OlcbCanLinkState 	state;
			unsigned long 	 	stateStopWait;
	
protected:
			void		nextAlias();	
	
			boolean 	sendCID7();
			boolean 	sendCID6();
			boolean 	sendCID5();
			boolean 	sendCID4();
			boolean 	sendRID();
			boolean 	sendAMD();
			boolean 	sendINC();
			boolean 	sendAME();
			boolean 	sendAMR();
			boolean 	sendDNI();
			
			boolean 	checkMessageSource(unsigned short mit, short alias); 
			boolean 	processMessage(unsigned short mit, short src, byte dataLength, byte* data);


	virtual void 		onMessage(CanMessage& message);

	virtual boolean		doPost(OlcbEvent& event, unsigned long waitMillis);
	
public:
						OlcbCanLink();
						OlcbCanLink(boolean debug);
						
			void	 	begin(OlcbNodeId& aNodeid, CanAccess& aCan);
			void		loop();
			
			boolean 	post(OlcbEvent& event);							  // same as waitMillis = OLCB_NO_WAIT
			boolean 	post(OlcbEvent& event, unsigned long waitMillis); // waitMillis: timeout(ms), OLCB_NO_WAIT, OLCB_FOREVER

						
	virtual boolean 	isReady();
};


inline boolean OlcbCanLink::post(OlcbEvent& event) {
	return OlcbLink::post(event);
}

inline boolean OlcbCanLink::post(OlcbEvent& event, unsigned long waitMillis) {
	return OlcbLink::post(event, waitMillis);
}

#endif // __OLCB_CAN_LINK_H__