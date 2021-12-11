byte outflows[] = {3,4,5,6,7,8,2};
byte inflows[] = {10,11,9};
byte diceMapa[] = {
          0b00000100, //1
          0b00100010, //2
          0b00100110, //3  
          0b01100011, //4     
          0b01100111, //5
          0b01111011, //6
          0b11111111, //7
  };

void setup() {
  for (int i = 0; i < 7; i++){
      pinMode(outflows[i], OUTPUT);
      
  }
  for (int i = 0; i < 3; i++){
      pinMode(inflows[i], OUTPUT);
  }

  
  clearDice();
}

void clearDice()
{
    for (int i = 0; i < 7; i++){
      digitalWrite(outflows[i], LOW); 
      
    }
    analogWrite(inflows[0], 255);
    analogWrite(inflows[1], 255);
    analogWrite(inflows[2], 255);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 7; i++){
      clearDice();
      for (int j = 0; j < 7; j++){
        digitalWrite(outflows[j], bitRead(diceMapa[i], j));
        //digitalWrite(outflows[j], i == j); 

      }
      for(byte k = 255; k > 0; k--){
          analogWrite(inflows[0], k);
          analogWrite(inflows[1], k);
          analogWrite(inflows[2], k);
          delay(3);
      }
      for(byte k = 0; k < 255; k++){
          analogWrite(inflows[0], k);
          analogWrite(inflows[1], k);
          analogWrite(inflows[2], k);
          delay(3);
      }
      delay(5);
  }
  clearDice();
  delay(2000);
}
