#include "DSE.h"

byte mac[] = {0x60, 0x52, 0xD0, 0x06, 0x68, 0x98};//P1AM-ETH MAC
IPAddress ip(192, 168, 137, 177);//P1AM-ETH IP

DSE modulo = DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 1");

void setup(){
  Serial.begin(115200);
  while(!Serial);
  Serial.println("#####INIT#####");

  Ethernet.begin(mac,ip);
  modulo.begin();
}

void loop(){
  if(modulo.connect()){
    modulo.update();
    Serial.print("Frecuencia: ");
    Serial.println(modulo.HZ);
  }
  else{
    Serial.println("Error trying to connect");
  }
  delay(2000);
}
