#include <Arduino.h>
#include <SPI.h>
#include <Can.h>
#include <Olcb.h>
#include <TextInput.h>

TextInput   TXT;
OlcbMCP2515 OLCB(10);

// Change this Node ID to your own
OlcbNodeId  NODE(2, 1, 0xD, 0xFF, 0xFF, 0x01);

OlcbNodeId  Brd0(0x02, 0x01, 0x36, 0x09, 0x0D, 0x00);
OlcbNodeId  Brd1(0x02, 0x01, 0x36, 0x09, 0x0D, 0x01);

void onReady(boolean ready) {
    Serial.print("OLCB: ");
    Serial.println(ready ? "ON" : "OFF");
}

void onEvent(OlcbEvent& event) {
    OlcbNodeId* nid = event.getSourceNodeId();
    if (nid != NULL) {
        nid->print(Serial);
    } else {
        short als = event.getSourceAlias();
        Serial.print("A:");
        Serial.print((als>>8) & 0xF,  HEX);
        Serial.print((als>>4) & 0xF,  HEX);
        Serial.print((als   ) & 0xF,  HEX);
    }
    Serial.print("->");
    event.println(Serial);
}

void onTextInput(const char* txt) {
    Serial.print(txt);
    Serial.println(OLCB.post(*OLCB.txtEvent(txt)) ? " - OK" : " - FAILED" );
}

void setup() {
    delay(100);
    Serial.begin(115200);
    
    OLCB.begin(NODE);
    OLCB.subscribe(onReady);
    OLCB.subscribe(onEvent);
    
    TXT.begin(Serial, 50);
    TXT.subscribe(onTextInput);   
    
    Brd0.trace();
    Brd1.trace();    
}

void loop() {
    OLCB.loop();
    TXT.loop();
}
