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
 
#ifndef __OLCB_EVENT_H__
#define __OLCB_EVENT_H__

#include "OlcbNodeId.h"

//Alias Map Definition (AMD) frame
#define OLCB_MIT_AMD	(0x0701)

//Alias Mapping Enquiry (AME) frame
#define OLCB_MIT_AME	(0x0702)

//Alias Map Reset (AMR) frame
#define OLCB_MIT_AMR	(0x0703)

//Initialization Complete (INC) frame
#define OLCB_MIT_INC	(0x9100)

//Producer/Consumer Event Report (PCER)
#define OLCB_MIT_PCER	(0x95B4)

//Producer Teach Event (PTE)
#define OLCB_MIT_PTE	(0x9594)

class OlcbEvent {
protected:
			short			sourceAlias;
			OlcbNodeId*		sourceNodeId;
			
			unsigned short	mti;
			byte  			data[8];

public:	
						OlcbEvent();
						OlcbEvent(unsigned short mti, byte* aData); //eight byte length
			     		OlcbEvent(unsigned short mti, byte data0, byte data1, byte data2, byte data3, byte data4, byte data5, byte data6, byte data7);
			
			void  		set(unsigned short mti, byte* aData);
			void  		set(unsigned short mti, byte data0, byte data1, byte data2, byte data3, byte data4, byte data5, byte data6, byte data7);
	
	//return null: sources are not tracked 
	//             if message hasn't been send
			void 		setSource(OlcbNodeId* nodeId);
			void 		setSource(short alias);
			
			OlcbNodeId* getSourceNodeId();
			short 		getSourceAlias();
			short		getMTI();
			byte* 	    getData(); 

			boolean 	equals(OlcbEvent* other);
			
			void 		print(Print& to);
			void		println(Print& to);
};

inline OlcbEvent::OlcbEvent(unsigned short aMTI, byte* aData) :
	sourceAlias(0),
	sourceNodeId(NULL) {
	set(aMTI, aData);
}

inline OlcbEvent::OlcbEvent(unsigned short aMTI, byte data0, byte data1, byte data2, byte data3, byte data4, byte data5, byte data6, byte data7) :
	sourceAlias(0),
	sourceNodeId(NULL) {
	set(aMTI, data0, data1, data2, data3, data4, data5, data6, data7);
}

inline void OlcbEvent::setSource(OlcbNodeId* nodeId) {
	sourceNodeId = nodeId;
	sourceAlias  = (nodeId == NULL ? 0 : nodeId->getAlias());
}

inline void OlcbEvent::setSource(short alias) {
	sourceNodeId = NULL;
	sourceAlias  = alias;
}

inline short OlcbEvent::getMTI() {
	return mti;
}

inline byte* OlcbEvent::getData() {
	return data;
}


#endif // __OLCB_EVENT_H__