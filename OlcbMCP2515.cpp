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
 #include "OlcbMCP2515.h"
 
OlcbMCP2515::OlcbMCP2515(byte spiCSpin) :
	OlcbCanLink(),
	_can(spiCSpin) {
}

OlcbMCP2515::OlcbMCP2515(byte spiCSpin, boolean debugCAN, boolean debugOlcb) :
	OlcbCanLink(debugOlcb),
	_can(spiCSpin, debugCAN) {
}