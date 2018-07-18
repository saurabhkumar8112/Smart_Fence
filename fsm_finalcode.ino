#define trigPin 5
#define echoPin 6
#define vccPin 4
#define gndPin 7

//Accelerometer GY 521
//A4: SDA; A5: SCL; VCC; Gnd

#include<Wire.h>
const int MPU=0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(gndPin, OUTPUT);
  pinMode(vccPin, OUTPUT);
  digitalWrite(gndPin, LOW);
  digitalWrite(vccPin, HIGH);

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
}

void loop() {
  // put your main code here, to run repeatedly:
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)/29.1;

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);
  int AcX_old=AcX;
  int AcY_old=AcY;

  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();

  //map(value, fromLow, fromHigh, toLow, toHigh)
  AcX=map(AcX, -32768, 32767, 180, -180);
  AcX=AcX-70;
  AcY=map(AcY, -32768, 32767, -180, 180);
  AcZ=map(AcZ, -32768, 32767, -180, 180);

  int deltaX=abs(AcX_old-AcX);
  int deltaY=abs(AcY_old-AcY);

  //************************************************
  //deltaX, deltaY and distance to be used in the Finite State Machine
  int res=0;
  String msg=" ";
  int accX=0;
  int accY=0;
  int ultra=0;
  int failure=0;
  //define ultra at 3 levels
  if(distance<75){
    ultra=2;
  }
  else if(distance<200){
    ultra=1;
  }
  else{
    ultra=0;
  }
  //define AcX at 3 levels
  if(deltaX>60){
    accX=2;
  }
  else if(deltaX>30){
    accX=1;
  }
  else{
    accX=0;
  }
  //define AcY at 3 levels
  if(deltaY>60){
    accY=2;
  }
  else if(deltaY>30){
    accY=1;
  }
  else{
    accY=0;
  }
  //Reading stops coming 2 level; code required
  failure=0;
  //
  if(failure==1){
    res=1;
  }
  else{
    if(accX==2 && accY==2 && ultra!=0){
      res=2;
    }
    else if(ultra!=0 && accX+accY>=2){
      res=3;
    }
    else if(ultra=0 && accX+accY>=3){
      res=4;
    }
    else if(accX+accY==0 && ultra==1){
      res=5; 
    }
    else if(accX+accY==0 && ultra==2){
      res=6;
    }
    else if(accX+accY>=1 && accX*accY==0 && ultra==2){
      res=7;
    }
    else if(accX+accY>=1 && accX*accY==0 && ultra==1){
      res=8;
    }
    else if(accX*accY*ultra==0){
      res=0;
    }
    else{
      res=9;
    }
  }

  switch(res){
    case 0:
      msg="Situation under control";
      break;
    case 1:
      msg="System failure, signal not coming";
      break;
    case 2:
      msg="Tresspassing and fence cutting attempt";
      break;
    case 3:
      msg="Tampering with fence";
      break;
    case 4:
      msg="Someone climbing the fence";
      break;
    case 5:
      msg="Something spotted at 2m distance";
      break;
    case 6:
      msg="Something spotted within a metre distance";
      break;
    case 7:
      msg="someonejJust touched the fence";
      break;
    case 8:
      msg="Seems a false alarm, but someone can be near look for further messages";
      break;
    case 9:
      msg="Low risk of someone above, but look for further messages";
      break;
    default:
      msg="Case left out";
  }
  //***********************************************
  //Serial Printing
  //In final product, only the variable msg will be printed.
  Serial.println("AcX : ");
  Serial.print(AcX);
  Serial.print(" deltaX: ");
  Serial.print(deltaX);
  Serial.print(" value of accX: ");
  Serial.print(accX);
  Serial.print(" AcY: ");
  Serial.print(AcY);
  Serial.print(" deltaY: ");
  Serial.print(deltaY);
  Serial.print(" value of accY: ");
  Serial.println(accY);
  //Serial.println("AcX : "+AcX+" deltaX: "+deltaX+" value of accX: "+accX+" AcY: "+AcY+" deltaY: "+deltaY+" value of accY: "+accY);
  Serial.println("Distance: ");
  Serial.print(distance);
  Serial.print(" value of ultra: ");
  Serial.println(ultra);
  //Serial.println("Distance: "+distance+" value of ultra: "+ultra);
  Serial.print(res);
  Serial.print(msg);
  //Serial.println(res+"   "+msg); 
  
  //************************************************
  delay(500);
}
