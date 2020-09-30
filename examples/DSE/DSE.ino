#include <DSE.h>

byte mac[] = {0x60, 0x52, 0xD0, 0x06, 0x68, 0x98};//P1AM-ETH MAC
IPAddress ip(192, 168, 137, 177);//P1AM-ETH IP


DSE modulos[9] = {
  DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 1"),
  DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 2"),
  DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 3"),
  DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 4"),
  DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 5"),
  DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 6"),
  DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 7"),
  DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 8"),
  DSE(DSE_8660MKII, IPAddress(192, 168, 137,  126), "Master 9")
};

void setup(){
  Serial.begin(115200);
  while(!Serial);
  Serial.println("#####INIT#####");

  Ethernet.begin(mac,ip);
  for(int i=0;i<9;i++){
    modulos[i].begin();
  }
}

void loop(){
  for(int i=0;i<9;i++){
    if(modulos[i].connect()){
      modulos[i].update();
      Serial.print(modulos[i].getName());
      Serial.print(" frecuencia: ");
      Serial.println(modulos[i].HZ);
      modulos[i].stop();
    }
    else{
      Serial.println("Error trying to connect");
    }
  }
  Serial.println(millis()/1000);
  delay(1000);
}
