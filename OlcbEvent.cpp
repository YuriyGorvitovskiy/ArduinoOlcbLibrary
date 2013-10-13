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
#include <OlcbEvent.h>

OlcbEvent::OlcbEvent() :
	sourceAlias(0),
	sourceNodeId(NULL),
	mti(0) {
	memset(this->data, 0, 8);
}

void OlcbEvent::set(unsigned short aMTI, byte* aData) {
	mti = aMTI;
	memcpy(data, aData, 8);
}

void OlcbEvent::set(unsigned short aMTI, byte data0, byte data1, byte data2, byte data3, byte data4, byte data5, byte data6, byte data7) {
	mti = aMTI;
	data[0] = data0;
	data[1] = data1;
	data[2] = data2;
	data[3] = data3;
	data[4] = data4;
	data[5] = data5;
	data[6] = data6;
	data[7] = data7;
}

OlcbNodeId* OlcbEvent::getSourceNodeId() {
	if (sourceNodeId != NULL)
		return sourceNodeId;
	return sourceNodeId = OlcbNodeId::find(sourceAlias);
}

short OlcbEvent::getSourceAlias() {
	if (sourceAlias != 0)
		return sourceAlias;
	if (sourceNodeId != NULL)
		return sourceAlias = sourceNodeId->getAlias();
	return 0;
}

boolean OlcbEvent::equals(OlcbEvent* other) {
	if (other == NULL)
		return false;
		
	if (this->mti != other->mti)
		return false;
		
	return memcmp(this->data, other->data, 8) == 0;
}

void OlcbEvent::print(Print& to) {
	to.print("E");
	switch (mti) {
		case OLCB_MIT_PCER:
			to.print("R");
			break;
		case OLCB_MIT_PTE:
			to.print("T");
			break;
		default:
			to.print(":");
			Serial.print((mti>>12) & 0xF,  HEX);
			Serial.print((mti>> 8) & 0xF,  HEX);
			Serial.print((mti>> 4) & 0xF,  HEX);
			Serial.print((mti    ) & 0xF,  HEX);
			break;
	}
	
	byte* data = getData();
	for (int i = 0; i < 8; ++i) {
    	Serial.print(i == 0 ? ":" : ".");
 	    Serial.print((data[i]>>4) & 0xF,  HEX);
     	Serial.print((data[i]   ) & 0xF,  HEX);
     }
}

void OlcbEvent::println(Print& to) {
	print(to);
	to.println("");
}
