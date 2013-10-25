//Alex Barchiesi for fog rings project 
// based partially on the code for max7313 by  eric toering
// Using the Wire library (created by Nicholas Zambetti)
// On the Arduino board, Analog In 4 is SDA, Analog In 5 is SCL

#include <Wire.h>
void postino(byte address, byte reg, byte data);

//2DO transform into array for multi max
byte max1 = 0x10;      // addressses of max chips
byte max2 = 0x11;
byte max3 = 0x12;
byte max4 = 0x13;      // addressses of max chips
byte max5 = 0x14;
byte max6 = 0x15;
byte max7 = 0x16;

int intensita = 0xff;   

#define BOXNUM 18  //number of boxes 16

int LEDSarray[BOXNUM];//numers of LED arrays = controller boards

int  LEDSON[BOXNUM]; //array for all on 
int  LEDSOFF[BOXNUM]; //array for all off 
int  LEDSLETTER[BOXNUM]; //array for all off 

//front 8 6 4 
int LEDSFRONT[BOXNUM];

//back 1 2 3 5 7 9
int LEDSBACK[BOXNUM];
int LEDS16[BOXNUM];
int LEDS712[BOXNUM];
int LEDS1318[BOXNUM];

//2DO transform into array for multi max 
byte LEDSstatus[BOXNUM]; //current machine status first 8 values correnspond to the 16 fog rings - second  8 to the motors

//byte chipdata = 0;

///debug vars
int debugT=00, Tdelay=250; //set debugT to 0 when not debuggin
int Tcharge=1000, Tshot=300; 
////
boolean LED13=false;
////

/////////////////////////////////////////////////////////////
//servo
/////////////////////////////////////////////////////////////
#define SD21Address  0x61 //The address of the SD21
int SERVO[10] = {
  0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47};




void setup(){

  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);
  for (byte i=max1; i< max5; i++){
    initMax(i);
    //    checkMax(i);   //checks with led
    pinMode(13, OUTPUT);     
  }
  delay(100);

  for(int i=0;i<BOXNUM;i++){
    LEDSarray[i]=0x00;
    LEDSstatus[i]=0x00;
    LEDS16[i]=0x00;
    LEDS712[i]=0x00;
    LEDS1318[i]=0x00;
  }
  for(int i=0;i<6;i++){
    LEDS16[remap(i+1)-1]=1;
    LEDS712[remap(i+1+6)-1]=1;
    LEDS1318[remap(i+1+12)-1]=1;
  }
  for(int i=0;i<BOXNUM;i++){
    LEDSON[i]=1;
    LEDSOFF[i]=0;
    LEDSFRONT[i]=0;
    LEDSBACK[i]=1;
  } 
  //front 8 6 4 
  LEDSFRONT[8]=1;
  LEDSFRONT[6]=1;
  LEDSFRONT[4]=1;

  //back are inverse than back (so 0 in the same place)
  LEDSBACK[8]=0;
  LEDSBACK[6]=0;
  LEDSBACK[4]=0;

  /////////////////////////////////////////////////////////////
  //servo
  /////////////////////////////////////////////////////////////
  for(int i = 0; i < 10; i++){
    Wire.beginTransmission(SD21Address);
    Wire.write(SERVO[i]);
    Wire.write(0);
    Wire.endTransmission();
  }
}

void loop()
{
  allOff();
  delay(100);
  ///////
  //shotAll();  
  sinlab();
  delay(1000);
  int input = Serial.read();  // read serial 
  int T=150; //ms
  
  
////receiving from processing  
  bool started, ended;
  int serialIn=0;
  byte YuanBuffer[18];
  int shotbuf[18];
  
   while(Serial.available() > 0)

////////////////////////from processing use something like:
//    myPort.write("<"); //ascii 0x3C
//    myPort.write(val);
//    myPort.write(">"); //ascii 0x3E
////////////////////////  to send the array

   {
//      char aChar = Serial.read();
      byte aChar = Serial.read();
     // if(aChar == '<')
      if(aChar == 0x3C)
      {
         started = true;
         ended = false;
      }
   //   else if(aChar == '>')
      else if(aChar == 0x3E )
      {
         ended = true;
         break; // Break out of the while loop
      }
      else
      {
         YuanBuffer[serialIn] = aChar; 
         serialIn++;
//         YuanBuffer[serialIn] = '\0'; //a string is a char terminated by \0 so take care if you use a char
      }
   }

   if(started && ended)
   {
      // We got a whole slice
      for(int i=1;i<BOXNUM;i++){
         shotbuf[i-1]=(int)YuanBuffer[i]; //casting
        }
      shot(shotbuf);
    
        // Do something else ??? :o
      serialIn = 0;
      started = false;
      ended = false;
   }
   else
   {
     // No data, or only some data, received what do we do ?
   }
  
  
  switch (input){
  case 1:  //If processing passes a '1' do case one 
    //shotSingle(1);
    //LED13!=LED13; // set the LED 
    Wire.beginTransmission(SD21Address);
    Wire.write(SERVO[0]);
    Wire.write(255);
    Wire.endTransmission();

    Wire.beginTransmission(SD21Address);
    Wire.write(SERVO[1]);
    Wire.write(255);
    Wire.endTransmission();

    Wire.beginTransmission(SD21Address);
    Wire.write(SERVO[2]);
    Wire.write(255);
    Wire.endTransmission();

    break;
  case 2: 
    //shotSingle(2);  
    //LED13!=LED13; // set the LED 
    //digitalWrite(13, LOW);
    Wire.beginTransmission(SD21Address);
    Wire.write(SERVO[0]);
    Wire.write(0);
    Wire.endTransmission();

    Wire.beginTransmission(SD21Address);
    Wire.write(SERVO[1]);
    Wire.write(0);
    Wire.endTransmission();

    Wire.beginTransmission(SD21Address);
    Wire.write(SERVO[2]);
    Wire.write(0);
    Wire.endTransmission();
    break;
  case 3:
    //shotSingle(3);  
    shotAll();
    LED13!=LED13; // set the LED


    //Wire.beginTransmission(SD21Address);
    //Wire.write(SERVO[2]);
    //Wire.write(255);  
    //Wire.endTransmission(); 


    break;
  case 4:         
    LED13!=LED13; // set the LED  
    break;
  case 5:         
    LED13!=LED13; // set the LED
    break;
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////// FUNCTIONS ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void postino(byte address, byte reg, byte data){  // small hint :) postino = italian for sender
  Wire.beginTransmission(address);
  Wire.write( reg); 
  Wire.write( data); 
  Wire.endTransmission();
}

//init max7313 
void initMax(byte maxi){
  postino(maxi, 0xf, 0x10);                  // output is high impedance (blink is disabled)
  postino(maxi, 0x6, 0x00);                  // input en output config.
  postino(maxi, 0x7, 0x00);                  // 0=out 1=in
  postino(maxi, 0x2, 0xff);                  // global intensity reg. for LED 0-7
  postino(maxi, 0x3, 0xff);                  // global intensity reg. for LED 8-15
  postino(maxi, 0xe, 0xff);
}

//blink all "leds" and check serial link
void checkMax(byte maxi)
{
  for (int i = 0x10; i < 0x18; i++){
    postino(maxi, i, 0x00);
    delay(100);
  }
  delay(100);
  for (int i = 0x10; i < 0x18; i++){
    postino(maxi, i, 0xff);
  }
  Serial.print("check");
}

// on and off correnspond to 0 and F for MAX7313 - because of pullup R 
// these functions takes care of all the logging and book keeping 
// do no take into account Pauses needed between one shot and the other
//consider them private not to be used directly

void shotpriv(int i, int j){
  if (j==0) shotOff(i);
  else if(j==1) shotFire(i);
}

void shotFire(int i){    //i always > 0
  if ((i % 2)>0){ //numeri dispari
    LEDSstatus[(i-1)/2] = (LEDSstatus[(i-1)/2]  & 0xF0) +0x0F; //1,3,5,7,9... // &0xF0 masks higher bit 
    if(i>=12){
      postino(max1, (0x10+(i-1-12)/2), LEDSstatus[(i-1-12)/2]);

    }
    else  {
      postino(max2, (0x10+(i-1)/2), LEDSstatus[(i-1)/2]); 
      //  postino(max2, (0x10+(i-1)/2), ((0x00 & 0xF0)+0x0F)); 
    }

  }
  else { //numeri pari 
    // postino(max2, (0x10+(i-1)/2), ((0x00 & 0x0F)+0xF0)); 
    LEDSstatus[(i-1)/2] = (LEDSstatus[(i-1)/2]  & 0x0F) +0xF0;//0,2,4,6,8... // &0x0F masks lower bit
    if(i>=12){
            postino(max2, (0x10+(i-1-12)/2), LEDSstatus[(i-1-12)/2]);

    }
    else{
      postino(max1, (0x10+(i-1)/2), LEDSstatus[(i-1)/2]);
    }
  }
}

void shotOff(int i){  
  if ((i % 2)>0){ //numeri dispari
    LEDSstatus[(i-1)/2] = (LEDSstatus[(i-1)/2]  | 0x0F ) -0x0F; // |0x0F masks higher bit
    postino(max2, (0x10+(i-1)/2),  LEDSstatus[(i-1)/2]); //1,3,5,7,9... 
    //postino(max2, (0x10+(i-1)/2),0x00);//0,2,4,6,8... 
  }
  else { //numeri pari e 0
    LEDSstatus[(i-1)/2] = (LEDSstatus[(i-1)/2]   | 0xF0) -0xF0; // |0xF0 masks lower bit
    postino(max2, (0x10+(i-1)/2),LEDSstatus[(i-1)/2]);//0,2,4,6,8... 
    // postino(max2, (0x10+(i-1)/2),0x00);//0,2,4,6,8... 
  }
}

///////////////////////////////
// these is the public function 
// array contains 0=off 1=on
void shot(int *array){
  for (int j=0; j<BOXNUM; j++) {
    shotpriv(j+1,array[j]);
  }
  delay(Tshot);
  for (int j=0; j<BOXNUM; j++) {
    shotpriv(j+1,0);
  }
  delay(Tshot); //dovrebbe essere Tcharge
}

void shotSingle(int i){
  int SINGLELED[BOXNUM];
  for(int h=0; h<BOXNUM; h++){  
    SINGLELED[h]=0;
  }
  SINGLELED[i-1]=1;
  shot(SINGLELED);
}

void shotAll(){ 
  shot(LEDSON);
}

void allOff(){
  shot(LEDSOFF);
}


void slice(int dimension,int *slice){//array con 0 e 1 
  for(int i=0 ; i < dimension/BOXNUM; i++){
    byte maxi=max1+i;
    for(int j=0; j<BOXNUM; j++){
      //    postino(maxi,(0x10)+i,slice(i,j))
    }
  }
  //  postino();
  delay(Tshot);
}

void cusp(int T){
  for(int j=1; j<=BOXNUM/2; j++)
  {  
    shotSingle(j);
    shotSingle(BOXNUM-j);
    delay(T);    
    //vertex ? solo se dispari
  }

};
void ramp(int T){
  for(int j=1; j<=BOXNUM; j++)
  {
    shotSingle(j);
    delay(T);    
  }
};

//
void s(){
  // 1 2 3 4 5 6
  shot(LEDS16);
  for(int h=0;h<3;h++) {
    shotSingle(remap(6)-1); 
  }
  shot(LEDS16);  
  for(int h=0;h<3;h++){
    shotSingle(remap(1)-1);
  }  
  shot(LEDS16);   
  shot(LEDS16);   
  for(int h=0;h<3;h++) {
    shotSingle(remap(1)-1);
  }
  shot(LEDS16);   
};

void i(){
  for(int i=0;i<BOXNUM;i++){
    LEDSLETTER[i]=0;
  }
  LEDSLETTER[remap(3)-1]=1;
  LEDSLETTER[remap(4)-1]=1;
  for(int h=0;h<7;h++)  shot(LEDSLETTER);
};

void n(){
  for(int i=0;i<BOXNUM;i++){
    LEDSLETTER[i]=0;
  }
  LEDSLETTER[remap(1)-1]=1;
  LEDSLETTER[remap(6)-1]=1;
  shot(LEDSLETTER);
  LEDSLETTER[remap(2)-1]=1;
  shot(LEDSLETTER);
  LEDSLETTER[remap(2)-1]=0;
  LEDSLETTER[remap(3)-1]=1;
  shot(LEDSLETTER);
  LEDSLETTER[remap(3)-1]=0;
  LEDSLETTER[remap(4)-1]=1;
  shot(LEDSLETTER);
  LEDSLETTER[remap(4)-1]=0;
  LEDSLETTER[remap(5)-1]=1;
  shot(LEDSLETTER);
};

void l(){
  for(int h=0;h<6;h++) shotSingle(remap(6)-1);
  shot(LEDS16);
};

void a(){
  for(int i=0;i<BOXNUM;i++){
    LEDSLETTER[i]=0;
  }
  LEDSLETTER[remap(1)-1]=1;
  LEDSLETTER[remap(6)-1]=1;
  shot(LEDS16);  
  shot(LEDSLETTER);
  shot(LEDS16);  
  shot(LEDSLETTER);
  shot(LEDSLETTER);
  shot(LEDSLETTER);
  shot(LEDSLETTER);
};

void b(){
  for(int i=0;i<BOXNUM;i++){
    LEDSLETTER[i]=0;
  }
  LEDSLETTER[remap(1)-1]=1;
  LEDSLETTER[remap(6)-1]=1;
  shot(LEDS16);  
  shot(LEDSLETTER);
  shot(LEDSLETTER);
  shot(LEDS16);  
  shot(LEDSLETTER);
  shot(LEDSLETTER);
  shot(LEDS16); 
};

void sinlab(){
  s();
  delay(1000);
  i();
  delay(1000);
  n();
  delay(1000);
  l();
  delay(1000);
  a();
  delay(1000);
  b();
  delay(1000);
}

int remap(int i){
  int j=0;
  if (i==8)  j=10;
  else if (i==9)  j=13; 
  else if (i==10)  j=14;
  else if (i==11) j=15;
  else if (i==12) j=16;
  else j=i;
  return j;
}


void shotBack();
void shotFront();



