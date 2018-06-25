//Accelerometer GY 521
//A4: SDA; A5: SDL; VCC, GND
#include<Wire.h>
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  //map(value, fromLow, fromHigh, toLow, toHigh)
  AcX=map(AcX, -32768, 32767, 180, -180);
  AcX=AcX-50;
  AcY=map(AcY, -32768, 32767, -180, 180);
  AcZ=map(AcZ, -32768, 32767, -180, 180);
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  //Serial.print(" | Z = "); Serial.println(AcZ); 
  
  //Serial.print("Gyroscope: ");
  //Serial.print("X = "); Serial.print(GyX);
  //Serial.print(" | Y = "); Serial.print(GyY);
  //Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");
  delay(2000);
}
