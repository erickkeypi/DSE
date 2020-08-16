
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

    mode = modbusTCPClient.holdingRegisterRead(772);
    priority = modbusTCPClient.holdingRegisterRead(35104);
    qualityMSC = modbusTCPClient.holdingRegisterRead(30);
    mastersOnline = modbusTCPClient.holdingRegisterRead(29);
    gensOnline = modbusTCPClient.holdingRegisterRead(28);

    busHZ = modbusTCPClient.holdingRegisterRead(1091);
    busP = modbusTCPClient.holdingRegisterRead(1606);
    busPhRot = modbusTCPClient.holdingRegisterRead(1118);

    busLLAVR = modbusTCPClient.holdingRegisterRead(1715) << 16 | modbusTCPClient.holdingRegisterRead(1714);
    busLNAVR = modbusTCPClient.holdingRegisterRead(1707) << 16 | modbusTCPClient.holdingRegisterRead(1706);
    busKW = modbusTCPClient.holdingRegisterRead(1585) << 16 | modbusTCPClient.holdingRegisterRead(1584);
    busL1N = modbusTCPClient.holdingRegisterRead(1093) << 16 | modbusTCPClient.holdingRegisterRead(1092);
    busL2N = modbusTCPClient.holdingRegisterRead(1095) << 16 | modbusTCPClient.holdingRegisterRead(1094);
    busL3N = modbusTCPClient.holdingRegisterRead(1097) << 16 | modbusTCPClient.holdingRegisterRead(1096);
    busL1L2 = modbusTCPClient.holdingRegisterRead(1099) << 16 | modbusTCPClient.holdingRegisterRead(1098);
    busL2L3 = modbusTCPClient.holdingRegisterRead(1101) << 16 | modbusTCPClient.holdingRegisterRead(1100);
    busL3L1 = modbusTCPClient.holdingRegisterRead(1103) << 16 | modbusTCPClient.holdingRegisterRead(1102);

    switch (model) {
      case DSE_8660MKII:
      HZ = modbusTCPClient.holdingRegisterRead(1059);
      P = modbusTCPClient.holdingRegisterRead(1582);
      PF = modbusTCPClient.holdingRegisterRead(1581);
      PhRot = modbusTCPClient.holdingRegisterRead(1074);

      oilPressure = modbusTCPClient.holdingRegisterRead(1024);
      battery = modbusTCPClient.holdingRegisterRead(1029);//battery
      engineSpeed = modbusTCPClient.holdingRegisterRead(1030);//engine speed
      coolantTemperature = modbusTCPClient.holdingRegisterRead(1025);//coolant temp
      fuelLevel = modbusTCPClient.holdingRegisterRead(1027);//fuel level

      V = modbusTCPClient.holdingRegisterRead(1699) << 16 | modbusTCPClient.holdingRegisterRead(1668);
      KW = modbusTCPClient.holdingRegisterRead(1801) << 16 | modbusTCPClient.holdingRegisterRead(1800);//revisar
      KVAR = modbusTCPClient.holdingRegisterRead(1577) << 16 | modbusTCPClient.holdingRegisterRead(1576);
      KVA = modbusTCPClient.holdingRegisterRead(1569) << 16 | modbusTCPClient.holdingRegisterRead(1568);
      LLAVR = modbusTCPClient.holdingRegisterRead(1699) << 16 | modbusTCPClient.holdingRegisterRead(1698);
      L1N = modbusTCPClient.holdingRegisterRead(1061) << 16 | modbusTCPClient.holdingRegisterRead(1060);
      L2N = modbusTCPClient.holdingRegisterRead(1063) << 16 | modbusTCPClient.holdingRegisterRead(1062);
      L3N = modbusTCPClient.holdingRegisterRead(1065) << 16 | modbusTCPClient.holdingRegisterRead(1064);
      L1L2 = modbusTCPClient.holdingRegisterRead(1067) << 16 | modbusTCPClient.holdingRegisterRead(1066);
      L2L3 = modbusTCPClient.holdingRegisterRead(1069) << 16 | modbusTCPClient.holdingRegisterRead(1068);
      L3L1 = modbusTCPClient.holdingRegisterRead(1071) << 16 | modbusTCPClient.holdingRegisterRead(1070);
      IL1 = modbusTCPClient.holdingRegisterRead(1077) << 16 | modbusTCPClient.holdingRegisterRead(1076);
      IL2 = modbusTCPClient.holdingRegisterRead(1079) << 16 | modbusTCPClient.holdingRegisterRead(1078);
      IL3 = modbusTCPClient.holdingRegisterRead(1081) << 16 | modbusTCPClient.holdingRegisterRead(1080);

      engineRuntime = modbusTCPClient.holdingRegisterRead(1799) << 16 | modbusTCPClient.holdingRegisterRead(1798);
      numberOfStarts = modbusTCPClient.holdingRegisterRead(1809) << 16 | modbusTCPClient.holdingRegisterRead(1808);

      mainsAvailable = modbusTCPClient.holdingRegisterRead(48658);
      genAvailable = 0;
      busAvailable = modbusTCPClient.holdingRegisterRead(48661);
      mainBrk = modbusTCPClient.holdingRegisterRead(48659);
      busBrk = modbusTCPClient.holdingRegisterRead(48660);
      genBrk = 0;
      busLive = 0;
      loadOn = (busBrk && busAvailable) | (mainBrk && mainsAvailable);
      break;

      case DSE_8610MKII://///////////////////////
      HZ = modbusTCPClient.holdingRegisterRead(1031);
      P = modbusTCPClient.holdingRegisterRead(1558);
      PF = modbusTCPClient.holdingRegisterRead(1557);
      PhRot = modbusTCPClient.holdingRegisterRead(1073);

      V = modbusTCPClient.holdingRegisterRead(1651) << 16 | modbusTCPClient.holdingRegisterRead(1650);
      KW = modbusTCPClient.holdingRegisterRead(1537) << 16 | modbusTCPClient.holdingRegisterRead(1536);
      KVAR = modbusTCPClient.holdingRegisterRead(1553) << 16 | modbusTCPClient.holdingRegisterRead(1552);
      KVA = modbusTCPClient.holdingRegisterRead(1545) << 16 | modbusTCPClient.holdingRegisterRead(1544);
      LLAVR = modbusTCPClient.holdingRegisterRead(1659) << 16 | modbusTCPClient.holdingRegisterRead(1658);
      L1N = modbusTCPClient.holdingRegisterRead(1033) << 16 | modbusTCPClient.holdingRegisterRead(1032);
      L2N = modbusTCPClient.holdingRegisterRead(1035) << 16 | modbusTCPClient.holdingRegisterRead(1034);
      L3N = modbusTCPClient.holdingRegisterRead(1037) << 16 | modbusTCPClient.holdingRegisterRead(1036);
      L1L2 = modbusTCPClient.holdingRegisterRead(1039) << 16 | modbusTCPClient.holdingRegisterRead(1038);
      L2L3 = modbusTCPClient.holdingRegisterRead(1041) << 16 | modbusTCPClient.holdingRegisterRead(1040);
      L3L1 = modbusTCPClient.holdingRegisterRead(1043) << 16 | modbusTCPClient.holdingRegisterRead(1042);
      IL1 = modbusTCPClient.holdingRegisterRead(1045) << 16 | modbusTCPClient.holdingRegisterRead(1044);
      IL2 = modbusTCPClient.holdingRegisterRead(1047) << 16 | modbusTCPClient.holdingRegisterRead(1046);
      IL3 = modbusTCPClient.holdingRegisterRead(1049) << 16 | modbusTCPClient.holdingRegisterRead(1048);

      mainsAvailable = 0;
      busAvailable = 0;
      genAvailable = modbusTCPClient.holdingRegisterRead(48661);
      mainBrk = 0;
      genBrk = modbusTCPClient.holdingRegisterRead(48659);
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
