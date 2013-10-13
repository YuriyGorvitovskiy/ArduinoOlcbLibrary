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

#ifndef __OLCB_MCP2515_H__
#define __OLCB_MCP2515_H__
 
#include "Can.h"
#include "OlcbCanLink.h"

class OlcbMCP2515 : public OlcbCanLink {
private:
	CanMCP2515 _can;

public:

	OlcbMCP2515(byte spiCSpin);
	OlcbMCP2515(byte spiCSpin, boolean debugCAN, boolean debugOlcb);
	
	void begin(OlcbNodeId& nodeId);
	void loop();
};

inline void OlcbMCP2515::begin(OlcbNodeId& nodeId) {
	_can.begin(Can125kbps, CanFilterOlcb, CanModeNormal);
	OlcbCanLink::begin(nodeId, _can);
}

inline void OlcbMCP2515::loop() {
	_can.loop();
	OlcbCanLink::loop();
}

#endif //__OLCB_MCP2515_H__