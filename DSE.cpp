
/*
Copyright 2019 Erick Rafael Garcia Martinez

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

last revision august/15/2020
*/

#include "DSE.h"

DSE::DSE(){
  for(int i=0;i<16;i++){
    name[i]=0;
  }
  for(int i=0;i<150;i++){
    alarms[i]=false;
  }
}

DSE::DSE(int _model, IPAddress _ip){
  DSE();
  model = _model;
  ip = _ip;
}

DSE::DSE(int _model, IPAddress _ip,char _name[]){
  DSE();
  model = _model;
  ip = _ip;
  strcpy(name,_name);
}

void DSE::begin(){
  modbusTCPClient.setTimeout(DSE_MODBUS_TIMEOUT);
}

bool DSE::connect(){
  commError = false;
  if(!modbusTCPClient.connected()){
    if(!modbusTCPClient.begin(ip)){
      commError = true;
    }
  }
  return !commError;
}

void DSE::setName(char _name[]){
  strcpy(name,_name);
}

char *DSE::getName(){
  return name;
}

void DSE::update(){
  if(commError){
    for (int i=0;i<150;i++){
      alarms[i]=false;
    }
  }

  if(!modbusTCPClient.connected()){
    connect();
  } else {
    if(modbusTCPClient.requestFrom(HOLDING_REGISTERS,39425,37)){//LEYENDO LOS REGITROS DE ALARM
      if(modbusTCPClient.available()){
        for(int i=0;i<37;i++){
          alarmsReg[i] = modbusTCPClient.read();//COPIANDO LAS ALARMAS EN EL ARRAY CORRESPONDIENTE
        }
        computeAlarms();
      }
    }
    else {
      commError=true;//SE ACTIVA EL ERROR SI SE DETECTA UN ERROR EN LA LECTURA DE LOS REGISTROS
    }

    if(modbusTCPClient.requestFrom(HOLDING_REGISTERS,1792,2)){
      if(modbusTCPClient.available()){
        time = modbusTCPClient.read() << 16 | modbusTCPClient.read();
      }
    }
    else {
      commError=true;
    }
  }

  if(!commError){

    gensOnline = modbusTCPClient.holdingRegisterRead(28);
    mastersOnline = modbusTCPClient.holdingRegisterRead(29);
    qualityMSC = modbusTCPClient.holdingRegisterRead(30);
    mode = modbusTCPClient.holdingRegisterRead(772);
    priority = modbusTCPClient.holdingRegisterRead(35104);

    busHZ = modbusTCPClient.holdingRegisterRead(1091);
    busL1N = modbusTCPClient.holdingRegisterRead(1093) << 16 | modbusTCPClient.holdingRegisterRead(1092);
    busL2N = modbusTCPClient.holdingRegisterRead(1095) << 16 | modbusTCPClient.holdingRegisterRead(1094);
    busL3N = modbusTCPClient.holdingRegisterRead(1097) << 16 | modbusTCPClient.holdingRegisterRead(1096);
    busL1L2 = modbusTCPClient.holdingRegisterRead(1099) << 16 | modbusTCPClient.holdingRegisterRead(1098);
    busL2L3 = modbusTCPClient.holdingRegisterRead(1101) << 16 | modbusTCPClient.holdingRegisterRead(1100);
    busL3L1 = modbusTCPClient.holdingRegisterRead(1103) << 16 | modbusTCPClient.holdingRegisterRead(1102);
    busPhRot = modbusTCPClient.holdingRegisterRead(1118);
    busKW = modbusTCPClient.holdingRegisterRead(1585) << 16 | modbusTCPClient.holdingRegisterRead(1584);
    busP = modbusTCPClient.holdingRegisterRead(1606);
    busLNAVR = modbusTCPClient.holdingRegisterRead(1707) << 16 | modbusTCPClient.holdingRegisterRead(1706);
    busLLAVR = modbusTCPClient.holdingRegisterRead(1715) << 16 | modbusTCPClient.holdingRegisterRead(1714);

    switch (model) {
      case DSE_8660MKII:

      HZ = modbusTCPClient.holdingRegisterRead(1059);
      L1N = modbusTCPClient.holdingRegisterRead(1061) << 16 | modbusTCPClient.holdingRegisterRead(1060);
      L2N = modbusTCPClient.holdingRegisterRead(1063) << 16 | modbusTCPClient.holdingRegisterRead(1062);
      L3N = modbusTCPClient.holdingRegisterRead(1065) << 16 | modbusTCPClient.holdingRegisterRead(1064);
      L1L2 = modbusTCPClient.holdingRegisterRead(1067) << 16 | modbusTCPClient.holdingRegisterRead(1066);
      L2L3 = modbusTCPClient.holdingRegisterRead(1069) << 16 | modbusTCPClient.holdingRegisterRead(1068);
      L3L1 = modbusTCPClient.holdingRegisterRead(1071) << 16 | modbusTCPClient.holdingRegisterRead(1070);
      PhRot = modbusTCPClient.holdingRegisterRead(1074);
      IL1 = modbusTCPClient.holdingRegisterRead(1077) << 16 | modbusTCPClient.holdingRegisterRead(1076);
      IL2 = modbusTCPClient.holdingRegisterRead(1079) << 16 | modbusTCPClient.holdingRegisterRead(1078);
      IL3 = modbusTCPClient.holdingRegisterRead(1081) << 16 | modbusTCPClient.holdingRegisterRead(1080);
      L1KW = modbusTCPClient.holdingRegisterRead(1085) << 16 | modbusTCPClient.holdingRegisterRead(1084);
      L2KW = modbusTCPClient.holdingRegisterRead(1087) << 16 | modbusTCPClient.holdingRegisterRead(1086);
      L3KW = modbusTCPClient.holdingRegisterRead(1089) << 16 | modbusTCPClient.holdingRegisterRead(1088);
      KW = modbusTCPClient.holdingRegisterRead(1561) << 16 | modbusTCPClient.holdingRegisterRead(1560);
      KVA = modbusTCPClient.holdingRegisterRead(1569) << 16 | modbusTCPClient.holdingRegisterRead(1568);
      KVAR = modbusTCPClient.holdingRegisterRead(1577) << 16 | modbusTCPClient.holdingRegisterRead(1576);
      PF = modbusTCPClient.holdingRegisterRead(1581);
      P = modbusTCPClient.holdingRegisterRead(1582);
      LLAVR = modbusTCPClient.holdingRegisterRead(1699) << 16 | modbusTCPClient.holdingRegisterRead(1698);
      V = modbusTCPClient.holdingRegisterRead(1691) << 16 | modbusTCPClient.holdingRegisterRead(1690);

      mainsAvailable = modbusTCPClient.holdingRegisterRead(48658);
      mainBrk = modbusTCPClient.holdingRegisterRead(48659);
      busBrk = modbusTCPClient.holdingRegisterRead(48660);
      busAvailable = modbusTCPClient.holdingRegisterRead(48661);
      genAvailable = 0;
      genBrk = 0;
      busLive = busAvailable;
      loadOn = (busBrk && busAvailable) | (mainBrk && mainsAvailable);
      break;

      case DSE_8610MKII://///////////////////////

      oilPressure = modbusTCPClient.holdingRegisterRead(1024);
      coolantTemperature = modbusTCPClient.holdingRegisterRead(1025);//coolant temp
      fuelLevel = modbusTCPClient.holdingRegisterRead(1027);//fuel level
      battery = modbusTCPClient.holdingRegisterRead(1029);//battery
      engineSpeed = modbusTCPClient.holdingRegisterRead(1030);//engine speed
      HZ = modbusTCPClient.holdingRegisterRead(1031);
      PhRot = modbusTCPClient.holdingRegisterRead(1073);
      L1N = modbusTCPClient.holdingRegisterRead(1033) << 16 | modbusTCPClient.holdingRegisterRead(1032);
      L2N = modbusTCPClient.holdingRegisterRead(1035) << 16 | modbusTCPClient.holdingRegisterRead(1034);
      L3N = modbusTCPClient.holdingRegisterRead(1037) << 16 | modbusTCPClient.holdingRegisterRead(1036);
      L1L2 = modbusTCPClient.holdingRegisterRead(1039) << 16 | modbusTCPClient.holdingRegisterRead(1038);
      L2L3 = modbusTCPClient.holdingRegisterRead(1041) << 16 | modbusTCPClient.holdingRegisterRead(1040);
      L3L1 = modbusTCPClient.holdingRegisterRead(1043) << 16 | modbusTCPClient.holdingRegisterRead(1042);
      IL1 = modbusTCPClient.holdingRegisterRead(1045) << 16 | modbusTCPClient.holdingRegisterRead(1044);
      IL2 = modbusTCPClient.holdingRegisterRead(1047) << 16 | modbusTCPClient.holdingRegisterRead(1046);
      IL3 = modbusTCPClient.holdingRegisterRead(1049) << 16 | modbusTCPClient.holdingRegisterRead(1048);
      L1KW = modbusTCPClient.holdingRegisterRead(1053) << 16 | modbusTCPClient.holdingRegisterRead(1052);
      L2KW = modbusTCPClient.holdingRegisterRead(1055) << 16 | modbusTCPClient.holdingRegisterRead(1054);
      L3KW = modbusTCPClient.holdingRegisterRead(1057) << 16 | modbusTCPClient.holdingRegisterRead(1056);
      KW = modbusTCPClient.holdingRegisterRead(1537) << 16 | modbusTCPClient.holdingRegisterRead(1536);
      KVA = modbusTCPClient.holdingRegisterRead(1545) << 16 | modbusTCPClient.holdingRegisterRead(1544);
      KVAR = modbusTCPClient.holdingRegisterRead(1553) << 16 | modbusTCPClient.holdingRegisterRead(1552);
      PF = modbusTCPClient.holdingRegisterRead(1557);
      P = modbusTCPClient.holdingRegisterRead(1558);
      V = modbusTCPClient.holdingRegisterRead(1651) << 16 | modbusTCPClient.holdingRegisterRead(1650);
      LLAVR = modbusTCPClient.holdingRegisterRead(1659) << 16 | modbusTCPClient.holdingRegisterRead(1658);
      engineRuntime = modbusTCPClient.holdingRegisterRead(1799) << 16 | modbusTCPClient.holdingRegisterRead(1798);
      KWH = modbusTCPClient.holdingRegisterRead(1801) << 16 | modbusTCPClient.holdingRegisterRead(1800);
      numberOfStarts = modbusTCPClient.holdingRegisterRead(1809) << 16 | modbusTCPClient.holdingRegisterRead(1808);

      genBrk = modbusTCPClient.holdingRegisterRead(48659);
      genAvailable = modbusTCPClient.holdingRegisterRead(48661);
      busAvailable = 0;
      mainsAvailable = 0;
      mainBrk = 0;
      busBrk = 0;
      busLive = genAvailable && genBrk;
      loadOn = busLive;
      break;
    }
  }
}

void DSE::computeAlarms(){
    for(int j=0;j<37;j++){
      //SEPARANDO
      int a1 = alarmsReg[j]&0b00001111;
      int a2 = (alarmsReg[j]>>4)&0b00001111;
      int a3 = (alarmsReg[j]>>8)&0b00001111;
      int a4 = (alarmsReg[j]>>12)&0b00001111;
      //DETECTANDO SI EL VALOR ESTA ENTRE 2 Y 4
      alarms[j*4]=a1<=4 && a1>=2;
      alarms[(j*4)+1]=a2<=4 && a2>=2;
      alarms[(j*4)+2]=a3<=4 && a3>=2;
      alarms[(j*4)+3]=a4<=4 && a4>=2;
    }
}

bool DSE::sendButton(int _button){
  if(!commError){
    if(modbusTCPClient.beginTransmission(HOLDING_REGISTERS,4104,2)){
      switch (_button) {
        case DSE_STOP_BUTTON:
        modbusTCPClient.write(35700);
        modbusTCPClient.write(29835);
        break;

        case DSE_AUTO_BUTTON:
        modbusTCPClient.write(35701);
        modbusTCPClient.write(29834);
        break;

        case DSE_MANUAL_BUTTON:
        modbusTCPClient.write(35702);
        modbusTCPClient.write(29833);
        break;

        case DSE_TEST_ON_LOAD_BUTTON:
        modbusTCPClient.write(35703);
        modbusTCPClient.write(29832);
        break;

        case DSE_START_BUTTON:
        modbusTCPClient.write(35705);
        modbusTCPClient.write(29830);
        break;

        case DSE_MUTE_BUTTON:
        modbusTCPClient.write(35706);
        modbusTCPClient.write(29829);
        break;

        case DSE_TRANSFER_TO_GEN_BUTTON:
        modbusTCPClient.write(35708);
        modbusTCPClient.write(29827);
        break;

        case DSE_TRANSFER_TO_MAIN_BUTTON:
        modbusTCPClient.write(35709);
        modbusTCPClient.write(29826);
        break;
      }
      modbusTCPClient.endTransmission();
      return true;
    } else{
      commError=true;
      return false;
    }
  }
  return false;
}

bool DSE::beginTransmission(unsigned int _add,unsigned int _num){
  if(!commError){
    return modbusTCPClient.beginTransmission(HOLDING_REGISTERS,_add,_num);
  }
  return false;
}

void DSE::modbusWrite(unsigned int _dat){
  modbusTCPClient.write(_dat);
}

void DSE::endTransmission(){
  modbusTCPClient.endTransmission();
}

void DSE::stop(){
  modbusTCPClient.stop();
}
