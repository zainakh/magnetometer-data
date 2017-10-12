#include <Time.h>
#include <Wire.h> //I2C Arduino Library

#define TCAADDR 0x70
#define addr 0x0D //I2C Address for The HMC5883

unsigned long timing;
int digitalPin = A1;
char receivedChar;
boolean start = false;

unsigned long previousMillis = 0;
const long interval = 10;

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setup() {

  pinMode(digitalPin, OUTPUT);
  Serial.begin(115200);
  Wire.begin();

  for(int j=0; j<4; j++){
    tcaselect(j); 
  
    Wire.beginTransmission(addr); //start talking
    Wire.write(0x0B); // Tell the HMC5883 to Continuously Measure
    Wire.write(0x01); // Set the Register
    Wire.endTransmission();
    
    Wire.beginTransmission(addr); //start talking
    Wire.write(0x09); // Tell the HMC5883 to Continuously Measure
    Wire.write(0x1D); // Set the Register
    Wire.endTransmission();
    
  }

  Serial.println("X0,Y0,Z0,X1,Y1,Z1,X2,Y2,Z2,X3,Y3,Z3,Time");
}

void loop() {

  int x, y, z; //triple axis data

  for(int i=0; i<4; i++)
  {
    tcaselect(i);
    
    //Tell the HMC what regist to begin writing data into
    Wire.beginTransmission(addr);
    Wire.write(0x00); //start with register 3.
    Wire.endTransmission();
    
    //Read the data.. 2 bytes for each axis.. 6 total bytes
    Wire.requestFrom(addr, 6);
    if (6 <= Wire.available()) {
      x = Wire.read(); //MSB  x
      x |= Wire.read() << 8; //LSB  x
      z = Wire.read(); //MSB  z
      z |= Wire.read() << 8; //LSB z
      y = Wire.read(); //MSB y
      y |= Wire.read() << 8; //LSB y
    }

    // Show Values
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.print(z);
    Serial.print(",");   
  }

  unsigned long currentMillis = millis();

  /*
  if(!start){
    receivedChar = Serial.read();
    if(receivedChar == 'a'){
      start = true;
    }
  }
  
  if(start){
    digitalWrite(digitalPin, HIGH);
    delay(1);
    digitalWrite(digitalPin, LOW);

    receivedChar = Serial.read();
    if(receivedChar == 'b'){
      start = false;
    }
  }
*/

  while((currentMillis - previousMillis) < interval){
    currentMillis = millis();
    //delayTime = interval - (currentMillis - previousMillis);
    //delay(delayTime);
  }

  previousMillis = currentMillis;
  Serial.print(currentMillis);
  Serial.print(",");
  Serial.println();
}
