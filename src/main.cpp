#include <Arduino.h>
#include <string>
#include <MyBuffer.h>
#include <driver/adc.h>

char             volatile matLabInput;
hw_timer_t                *pTimer;
hw_timer_t                *pTimer2;
unsigned int     volatile nISRCallCounter;
byte                      data[2];
uint16_t                  valueSent;    //2 byte
bool             volatile bLEDState;
bool             volatile interruptTimer2Control;
bool                      increase;
const int                 potIn = 36;   //input pin for adc signal
MyBuffer                  *myBuffer;



void onTimer2(){
  if (interruptTimer2Control){
    bLEDState = false;
    uint16_t value = myBuffer->pop();
    data[0] = (byte) (value & 0xFF);
    data[1] = (byte) ((value >> 8) & 0xFF);
    if (Serial.availableForWrite()){
        Serial.write(data, 2);
    } 
  } else {
        bLEDState = true;  
    }

  digitalWrite(LED_BUILTIN, bLEDState); 
}

void IRAM_ATTR onTimer() {
  matLabInput = Serial.read();
  switch (matLabInput)
  {
  case 'S':
    interruptTimer2Control = true;
    break;
  case 'T':
    interruptTimer2Control = false;
    break;

  default:
    break;
  }
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  bLEDState = 0;
  digitalWrite(LED_BUILTIN, bLEDState);

  nISRCallCounter = 0;
  bLEDState = false;
  interruptTimer2Control = false;
  valueSent = 0;
  increase = true;

  //setup buffer
  myBuffer = new MyBuffer(5000);

  pTimer = timerBegin(0, 40000, true); // Prescaler of 40.000 => count up in steps of 0.5 milliseconds
  timerAttachInterrupt(pTimer, &onTimer, true);
  timerAlarmWrite(pTimer, 1000, true); // Timer IR every 500 ms
  timerAlarmEnable(pTimer);

  pTimer2 = timerBegin(1, 80, true);
  timerAttachInterrupt(pTimer2, &onTimer2, true);
  timerAlarmWrite(pTimer2, 10000, true);    //Timer IR every 10ms
  timerAlarmEnable(pTimer2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.availableForWrite()){
    int value = adc1_get_raw(ADC1_CHANNEL_0) % 65535;
    myBuffer->push(value);
  }
}