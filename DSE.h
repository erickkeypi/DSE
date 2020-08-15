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

#ifndef _DSE_H
#define _DSE_H

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include <Ethernet.h>

#define DSE_8660MKII 0
#define DSE_8610MKII 1
#define DSE_MODBUS_TIMEOUT 200
#define DSE_STOP_BUTTON 0
#define DSE_START_BUTTON 1
#define DSE_TEST_ON_LOAD_BUTTON 2
#define DSE_MANUAL_BUTTON 3
#define DSE_AUTO_BUTTON 4
#define DSE_MUTE_BUTTON 5
#define DSE_TRANSFER_TO_MAIN_BUTTON 6
#define DSE_TRANSFER_TO_GEN_BUTTON 7

class DSE {
private:
  int model;
  char name[16];
  int alarmsReg[37];
  IPAddress ip;
  EthernetClient ethernetClient;
  ModbusTCPClient modbusTCPClient = ModbusTCPClient(ethernetClient);

  void computeAlarms();
  DSE();

public:
  bool commError, commonAlarm;
  bool mainsAvailable, busAvailable, mainBrk, busBrk, genBrk, busLive, genAvailable, loadOn;
  bool alarms[150];
  unsigned int HZ, P, mode, busHZ, busP, PF, PhRot, busPhRot, priority, qualityMSC, mastersOnline, gensOnline;
  unsigned int oilPressure, Battery, engineSpeed, coolantTemperature, fuelLevel;
  unsigned long engineRuntime, numberOfStarts;
  unsigned long V, KW, KVAR, KVA, LLAVR, L1N, L2N, L3N, L1L2, L2L3, L3L1, IL1, IL2, IL3 ;
  unsigned long busKW, busLNAVR,busLLAVR, busL1N, busL2N, busL3N, busL1L2, busL2L3, busL3L1;

  DSE(int);
  DSE(int, IPAddress);
  DSE(int, IPAddress,char[]);
  void begin();
  bool connect();
  void setName(char[]);
  char *getName();
  void update();
  bool sendButton(int);
};
#endif