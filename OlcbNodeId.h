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
 
#ifndef __OLCB_NODE_ID_H__
#define __OLCB_NODE_ID_H__

#include <Arduino.h>

class OlcbNodeId {
private:
	static 	OlcbNodeId*  	headToTrace; // Tracking Linked List
			OlcbNodeId* 	nextToTrace; // Tracking Linked List

		   	byte  			data[6];

	//alias is stored here to avoid any memory allocation for mapping;
			short 			alias;
			
public:
						OlcbNodeId(byte* aData);
						OlcbNodeId(byte data0, byte data1, byte data2, byte data3, byte data4, byte data5);

	static  OlcbNodeId* find(short alias);
			void 		trace();	

	static  OlcbNodeId* resolve(byte* data, short alias);
			void		resolve(short alias);

			byte*		getData();
			short		getAlias();
	
			
			boolean 	equals(OlcbNodeId* other);
			boolean 	equals(byte* data);
	
			void 		print(Print& to);
			void		println(Print& to);
};

inline void  OlcbNodeId::resolve(short anAlias) {
	alias = anAlias;
}

inline short OlcbNodeId::getAlias() {
	return alias;
}

inline byte* OlcbNodeId::getData() {
	return data;
}

#endif // __OLCB_NODE_ID_H__