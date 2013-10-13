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
#include <Print.h>
#include "OlcbNodeId.h"

OlcbNodeId* OlcbNodeId::headToTrace = NULL; // Tracking Linked List

OlcbNodeId::OlcbNodeId(byte* aData) :
	nextToTrace(NULL),
	alias(0) {
	
	memcpy(data, aData, 6);
}

OlcbNodeId::OlcbNodeId(byte data0, byte data1, byte data2, byte data3, byte data4, byte data5)  :
	nextToTrace(NULL),
	alias(0) {
	
	data[0] = data0;
	data[1] = data1;
	data[2] = data2;
	data[3] = data3;
	data[4] = data4;
	data[5] = data5;
}

OlcbNodeId* OlcbNodeId::find(short alias) {
	if (alias == 0)
		return NULL;
		
	for (OlcbNodeId* p = headToTrace; p != NULL; p = p->nextToTrace) {
		if (p->alias == alias)
			return p;
	}
	return NULL;
}

void OlcbNodeId::trace() {
	if (headToTrace == NULL) {
		headToTrace = this;
		return;
	}
	OlcbNodeId* p = headToTrace;
	while(p->nextToTrace != NULL)
		p = p->nextToTrace;
	
	p->nextToTrace = this;
}

OlcbNodeId* OlcbNodeId::resolve(byte* data, short alias) {
	OlcbNodeId* resolved = NULL;
	for (OlcbNodeId* p = headToTrace; p != NULL; p = p->nextToTrace) {
		if (memcmp(p->data, data, 6) == 0) {
			p->resolve(alias);
			resolved = p;
		}
	}
	return resolved;
}

boolean OlcbNodeId::equals(OlcbNodeId* other) {
	if (other == NULL)
		return false;
	return memcmp(this->data, other->data, 6) == 0;
}

boolean OlcbNodeId::equals(byte* nidData) {
	if (nidData == NULL)
		return false;
	return memcmp(this->data, nidData, 6) == 0;
}

void OlcbNodeId::print(Print& to) {
	to.print("N:");
	byte* data = getData();
	for (int i = 0; i < 6; ++i) {
         if (i != 0)
             to.print(".");
         to.print(data[i] >> 4,  HEX);
         to.print(data[i] & 0XF, HEX);
     }
}

void OlcbNodeId::println(Print& to) {
	print(to);
	to.println("");
}
