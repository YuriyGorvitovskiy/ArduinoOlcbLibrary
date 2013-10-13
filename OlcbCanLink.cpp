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
#include "OlcbLink.h"
#include "OlcbCanLink.h"

#define OLCB_MESSAGE    (0x10000000L)
#define OLCB_MIT_MASK 	(0xFFFF000L)
#define OLCB_MIT_SHFT 	(12)
#define OLCB_SRC_MASK 	(0xFFF)

#define OLCB_CID7_EID 	(0x17000000L)
#define OLCB_CID6_EID	(0x16000000L)
#define OLCB_CID5_EID	(0x15000000L)
#define OLCB_CID4_EID	(0x14000000L)

#define OLCB_RID_EID	(0x10700000L)
#define OLCB_AMD_EID	(OLCB_MESSAGE | (((long)OLCB_MIT_AMD) << OLCB_MIT_SHFT))
#define OLCB_AME_EID	(OLCB_MESSAGE | (((long)OLCB_MIT_AME) << OLCB_MIT_SHFT))
#define OLCB_AMR_EID	(OLCB_MESSAGE | (((long)OLCB_MIT_AMR) << OLCB_MIT_SHFT))
#define OLCB_INC_EID	(OLCB_MESSAGE | (((long)OLCB_MIT_INC) << OLCB_MIT_SHFT))
#define OLCB_PCER_EID	(OLCB_MESSAGE | (((long)OLCB_MIT_PCER)<< OLCB_MIT_SHFT))

static byte	 DUPLICATE_NODE_ID_EVENT_ID[8] = {1,1,0,0,0,0,2,1};

OlcbCanLink* OlcbCanLink::nodeIdResolver = NULL;

OlcbCanLink::OlcbCanLink() :
	OlcbLink(false),
	state(OlcbCanLinkInhibited),
	stateStopWait(0L) {
	aliasGenerator[0] = 0;
	aliasGenerator[0] = 0;
	aliasGenerator[0] = 0;
	aliasGenerator[0] = 0;
}

OlcbCanLink::OlcbCanLink(boolean debug) :
	OlcbLink(debug),
	state(OlcbCanLinkInhibited),
	stateStopWait(0L) {
	aliasGenerator[0] = 0;
	aliasGenerator[0] = 0;
	aliasGenerator[0] = 0;
	aliasGenerator[0] = 0;
}

void OlcbCanLink::begin(OlcbNodeId& aNodeId, CanAccess& aCan) {
	OlcbLink::begin(aNodeId);
	CanConsumer::begin(aCan);
	if (nodeIdResolver == NULL)
		nodeIdResolver = this;
		
	state = OlcbCanLinkInhibited;
	stateStopWait  = 0L;
	
	// each short has 12(low) significant bits;	
	byte* nid = nodeId->getData();
	aliasGenerator[0] = ((((short)nid[0]) << 4) | (nid[1] >> 4));
	aliasGenerator[1] = ((((short)nid[1]) << 8) |  nid[2]);
	aliasGenerator[2] = ((((short)nid[3]) << 4) | (nid[4] >> 4));
	aliasGenerator[3] = ((((short)nid[4]) << 8) |  nid[5]);
	
	nextAlias();
	DEBUG("OLCB Begin");
}

void OlcbCanLink::nextAlias() {
	do {
		//x(i+1) = (2^9 + 1) * x(i) + 0x1B0CA37A4BA9;
		//x(i+1) = (x(i) << 9) + x(i) + 0x1B0CA37A4BA9;
	
		// each short has 12(low) significant bits;	
		short t[4];
		t[0] = ((aliasGenerator[0]<<9) | (aliasGenerator[1]>>7)) & 0xFFF;
		t[1] = ((aliasGenerator[1]<<9) | (aliasGenerator[2]>>7)) & 0xFFF;
		t[2] = ((aliasGenerator[2]<<9) | (aliasGenerator[3]>>7)) & 0xFFF;
		t[3] =  (aliasGenerator[3]<<9)                           & 0xFFF;
		
		aliasGenerator[3] += t[3] + 0xBA9;
		aliasGenerator[2] += t[2] + 0x7A4 + (aliasGenerator[3] >> 12);
		aliasGenerator[1] += t[1] + 0xCA3 + (aliasGenerator[2] >> 12);
		aliasGenerator[0] += t[0] + 0x1B0 + (aliasGenerator[1] >> 12);
		
		aliasGenerator[0] &= 0xFFF;
		aliasGenerator[1] &= 0xFFF;		
		aliasGenerator[2] &= 0xFFF;		
		aliasGenerator[3] &= 0xFFF;		
		
		nodeId->resolve(aliasGenerator[0] ^ aliasGenerator[0] ^ aliasGenerator[2] ^ aliasGenerator[3]);
	} while(nodeId->getAlias() == 0);
}

boolean OlcbCanLink::sendCID7() {
	byte* nid = nodeId->getData(); 
	long  nidpart = (((((long)nid[0]) << 16 | ((long)nid[1]) << 8)) & 0xFFF000);
	
	return CanConsumer::send(can->extMessage(OLCB_CID7_EID | nidpart | nodeId->getAlias(), 0, NULL));
}

boolean OlcbCanLink::sendCID6() {
	byte* nid = nodeId->getData(); 
	short  nidpart = (((((long)nid[1]) << 20 | ((long)nid[2]) << 12)) & 0xFFF000);
	
	return CanConsumer::send(can->extMessage(OLCB_CID6_EID | nidpart | nodeId->getAlias(), 0, NULL));
}

boolean OlcbCanLink::sendCID5() {
	byte* nid = nodeId->getData(); 
	long  nidpart = (((((long)nid[3]) << 16 | ((long)nid[4]) << 8)) & 0xFFF000);
	
	return CanConsumer::send(can->extMessage(OLCB_CID5_EID | nidpart | nodeId->getAlias(), 0, NULL));
}

boolean OlcbCanLink::sendCID4() {
	byte* nid = nodeId->getData(); 
	long  nidpart = (((((long)nid[4]) << 20 | ((long)nid[5]) << 12)) & 0xFFF000);
	
	return CanConsumer::send(can->extMessage(OLCB_CID4_EID | nidpart | nodeId->getAlias(), 0, NULL));
}

boolean OlcbCanLink::sendRID() {
	return CanConsumer::send(can->extMessage(OLCB_RID_EID | nodeId->getAlias(), 0, NULL), CAN_FOREVER);
}

boolean OlcbCanLink::sendAMD() {
	return CanConsumer::send(can->extMessage(OLCB_AMD_EID | nodeId->getAlias(), 0, NULL), CAN_FOREVER);
}

boolean OlcbCanLink::sendAME() {
	return CanConsumer::send(can->extMessage(OLCB_AME_EID | nodeId->getAlias(), 0, NULL), CAN_FOREVER);
}

boolean OlcbCanLink::sendAMR() {
	return CanConsumer::send(can->extMessage(OLCB_AMR_EID | nodeId->getAlias(), 0, NULL), CAN_FOREVER);
}

boolean OlcbCanLink::sendINC() {
	return CanConsumer::send(can->extMessage(OLCB_INC_EID | nodeId->getAlias(), 0, NULL), CAN_FOREVER);
}

boolean OlcbCanLink::sendDNI() {
	return CanConsumer::send(can->extMessage(OLCB_PCER_EID| nodeId->getAlias(), 8, DUPLICATE_NODE_ID_EVENT_ID), CAN_FOREVER);
}

boolean	OlcbCanLink::doPost(OlcbEvent& event, unsigned long waitMillis) {
	long eid = OLCB_MESSAGE | (((long)event.getMTI()) << OLCB_MIT_SHFT) | nodeId->getAlias();
	boolean success  = CanConsumer::post(can->extMessage(eid, 8, event.getData()), waitMillis);
	
	if (debugEnabled) {
		event.print(Serial);
		Serial.println(success ? "-OK" : "-FAIL");
	}
	return success;
}

boolean OlcbCanLink::isReady() {
	return state == OlcbCanLinkPermitted;
}

void OlcbCanLink::loop() {
	if (state == OlcbCanLinkUnusable || state == OlcbCanLinkPermitted)
		return;
		
	switch(state) {
		case OlcbCanLinkInhibited:
			if (sendCID7())
				state = OlcbCanLinkInhibited_CID7_Sent;
			break;
		case OlcbCanLinkInhibited_CID7_Sent:
			if (sendCID6())
				state = OlcbCanLinkInhibited_CID6_Sent;
			break;
		case OlcbCanLinkInhibited_CID6_Sent:
			if (sendCID5())
				state = OlcbCanLinkInhibited_CID5_Sent;
			break;
		case OlcbCanLinkInhibited_CID5_Sent:
			if (sendCID4()) 
				state = OlcbCanLinkInhibited_CID4_Sent;
			break;
		case OlcbCanLinkInhibited_CID4_Sent:
			if (sendRID()) {
				state = OlcbCanLinkInhibited_RID_Sent;
				stateStopWait = millis() + 200;
			}
			break;
		case OlcbCanLinkInhibited_RID_Sent: {
			if (stateStopWait<millis()) {
				sendAMD();
				sendINC();
				if (nodeIdResolver == this) 
					sendAME();
				state = OlcbCanLinkPermitted;
				DEBUG("OLCB Ready");
				handleReady(true);
			}
			break;
		}
		default:
			break;
	}
}

void OlcbCanLink::onMessage(CanMessage& message) {
	long eid = message.getEID();
	if ((eid & OLCB_MESSAGE) == 0L)
		return;
		
	unsigned short mit = (eid & OLCB_MIT_MASK) >> OLCB_MIT_SHFT;
	short src = (eid & OLCB_SRC_MASK);
	if (!checkMessageSource(mit, src))
		return;
	
	if (processMessage(mit, src, message.getDataLength(), message.getData()))
		return;
	
	rxEvent.set(mit, message.getData());
	rxEvent.setSource(src);
	handleEvent(rxEvent, NULL);
	if (debugEnabled) {
		Serial.print("IN  ");
		rxEvent.println(Serial);
	}
}

boolean OlcbCanLink::checkMessageSource(unsigned short mit, short src) {
	if(src != nodeId->getAlias())
		return true;
	
	boolean notify = false;
	if (state == OlcbCanLinkPermitted) {
		byte top = (byte)(mit >> 12);

		//Check if this is CID message: 0x7XXX, 0x6XXX, 0x5XXX, 0x4XXX, 0x3XXX, 0x2XXX, 0x1XXX
		if (((top & 0x80) == 0) && ((top & 0x7) != 0)) {
			sendRID();
			return true;
		}
		sendAMR();
		notify = true;
	} 
	
	nextAlias();
	state = OlcbCanLinkInhibited;
	DEBUG("Reset: !Alias!");
	if (notify)
		handleReady(false);
		
	return false;
}

boolean OlcbCanLink::processMessage(unsigned short mit, short src, byte dataLength, byte* data) {
	switch(mit) {
		case OLCB_MIT_AME: {
			if (dataLength == 0 || nodeId->equals(data))
				sendAMD();
			return true;
		}
		case OLCB_MIT_INC:
		case OLCB_MIT_AMD: {
			if (nodeId->equals(data)) {
				sendDNI();
				state = OlcbCanLinkUnusable;
				DEBUG("STOP: !NodeId!");
				handleReady(false);
				return true;
			}
			OlcbNodeId* rslv = OlcbNodeId::resolve(data, src);
			if(debugEnabled && rslv != NULL) {
				rslv->print(Serial);
				Serial.print("<->A:");
				Serial.println(rslv->getAlias(), HEX);
			}
			return true;
		}
		case OLCB_MIT_AMR: {
			OlcbNodeId::resolve(data, 0);
			return true;
		}
	}
	return false;
}
