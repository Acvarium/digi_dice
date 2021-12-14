#include <EEPROM.h>

byte outflows[] = {A2,4,5,6,7,8,A3};
byte inflows[] = {10,11,9};
byte brightness = 200;
int brightnessDir = 1;
byte blinkSpeed = 1;
bool brightMode = true;
long cycleCounter = 0;
byte diceValues[] = {0,0,0};
long volatile  shakeCounter = 0;
const byte shakePin = 2;

unsigned long  volatile lastShakeStamp = 0;
unsigned long  volatile firstShakeStamp = 0;
unsigned int volatile seedValue = 0;
bool volatile shakeContinues = false;

byte diceMapa[] = {
          0b00000100, //1
          0b00100010, //2
          0b00100110, //3  
          0b01100011, //4     
          0b01100111, //5
          0b01111011, //6
          0b11111111, //7
  };


void clearDice()
{
    for (int i = 0; i < 7; i++){
      digitalWrite(outflows[i], LOW); 
    }
    analogWrite(inflows[0], 255);
    analogWrite(inflows[1], 255);
    analogWrite(inflows[2], 255);
}


void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}


int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}


void setup() {
  Serial.begin(9600);
  pinMode(shakePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(shakePin), shaked, CHANGE);
  
  for (int i = 0; i < 7; i++){
      pinMode(outflows[i], OUTPUT);
      
  }
  for (int i = 0; i < 3; i++){
      pinMode(inflows[i], OUTPUT);
  }
  seedValue = readIntFromEEPROM(1);
  randomSeed(seedValue);
  
  clearDice();
}

void shaked(){
  if (!shakeContinues)
  {
      firstShakeStamp =  millis();
  }
  shakeContinues = true;
  
  shakeCounter++;
  Serial.println(shakeCounter);
  lastShakeStamp = millis();
  seedValue += lastShakeStamp;
}



void showValues(byte A, byte B, byte C){
  byte values[] = {A,B,C};

  for (int i = 0; i < 3; i++){
      for (int i = 0; i < 7; i++){
          digitalWrite(outflows[i], LOW); 
      }
      
      for(int j = 0; j < 3; j++){
          if (i == j)
              analogWrite(inflows[j], 0);
          else
              digitalWrite(inflows[j], HIGH);
      }
      if (!brightMode)
        delay(3);
      for (int k = 0; k < 7; k++){
          digitalWrite(outflows[k], bitRead(diceMapa[values[i]], k));
      }
      if (brightMode)
        delay(3);
  }
  
}

void loop() {
  if (lastShakeStamp - firstShakeStamp > 1000 && shakeContinues)
  {
      if (cycleCounter > 50)
          cycleCounter = 0; 
      if (millis() - lastShakeStamp > 500)
      {
        shakeContinues = false;
        randomSeed(seedValue);
        writeIntIntoEEPROM(1, seedValue);
      } 
             
  }
  showValues(diceValues[0], diceValues[1], diceValues[2]);
  cycleCounter++;
  if (cycleCounter < 300) {
      if (cycleCounter%3 == 0) {
          for (int i = 0; i < 3; i++) {
              diceValues[i] = random(6);
          }
      }
  }
//  else if (cycleCounter > 1000) {
//      cycleCounter = 0;
//  }


//  int newBrightness = brightness + brightnessDir * blinkSpeed;
//
//  if (newBrightness >= 255){
//      brightnessDir = -1;
//      brightness = 255;
//  }
//  else if(newBrightness <= 0){
//      brightnessDir = 1;
//      brightness = 0;
//  }
//  else {
//      brightness = newBrightness;
//  }

//  brightMode = brightness > 128;
  
  
}
