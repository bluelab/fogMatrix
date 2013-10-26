//Alex Barchiesi for fog rings project 
// based partially on the code for max7313 by  eric toering
// Using the Wire library (created by Nicholas Zambetti)
// On the Arduino board, Analog In 4 is SDA, Analog In 5 is SCL

#include <Wire.h>
void postino(byte address, byte reg, byte data);

//2DO transform into array for multi max
byte max2 = 0x6F;      // addressses of max chips
byte max1 = 0x11;
byte max3 = 0x12;
byte max4 = 0x13;      // addressses of max chips
byte max5 = 0x14;
byte max6 = 0x15;
byte max7 = 0x16;

int intensita = 0xff;   

#define BOXNUM 18  //number of boxes 16
#define NUMSLICES 8 // number of "slices" = height

int LEDSarray[BOXNUM];//numers of LED arrays = controller boards

int  LEDSON[BOXNUM]; //array for all on 
int  LEDSOFF[BOXNUM]; //array for all off 
int  LEDSLETTER[BOXNUM]; //array for all off 

int LEDS16[BOXNUM];
int LEDS712[BOXNUM];
int LEDS1318[BOXNUM];
int YuanBuffer[BOXNUM];


//2DO transform into array for multi max 
byte LEDSstatusMax2[BOXNUM], LEDSstatusMax1[BOXNUM]; //current machine status first 8 values correnspond to the 16 fog rings - second  8 to the motors

//byte chipdata = 0;

///debug vars
int debugT=00, Tdelay=150; //set debugT to 0 when not debuggin
int Tcharge=1000, Tshot=100; 

int ss[BOXNUM];
int shotbuf[BOXNUM][NUMSLICES];



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

  initMax(max1);
  initMax(max2);
  delay(100);

  for(int i=0;i<BOXNUM;i++){
    LEDSarray[i]=0x00;
    LEDSstatusMax2[i]=0x00; 
    LEDSstatusMax1[i]=0x00;
    LEDS16[i]=0x00;
    LEDS712[i]=0x00;
    LEDS1318[i]=0x00;
    YuanBuffer[i]=0x00;
    ss[i]=0x00;

  }
  for(int j=0;j<NUMSLICES;j++){
    for(int i=0;i<BOXNUM;i++){
      shotbuf[i][j]=0x00;
    }
  }

  for(int i=0;i<6;i++){
    LEDS16[i]=1;
    LEDS712[i+6]=1;
    LEDS1318[i+12]=1;
  }
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

void loop(){
  allOff();
  ///////
  // shotAll();
  // sinlab();
  // delay(1000);
  // int input = Serial.read();  // read serial 
  int T=150; //ms
  ////receiving from processing  
  bool Rstarted=false, Rended=false, Mstarted=false, Mended=false;
  int CserialIn=0, RserialIn=0;

  while(Serial.available() > 0){
    ////////////////////////from processing use something like:
    //    myPort.write("{"); //ascii 0x7B
    //    myPort.write("<"); //ascii 0x3C
    //    myPort.write(val);
    //    myPort.write(">"); //ascii 0x3E
    //    myPort.write("}"); //ascii 0x7D 
    ////////////////////////  to send the array
    //<111000111000111000>
    int aChar = Serial.read();
    //////init char "{"
    if(aChar == 0x7B){
      Mstarted = true;
      Mended = false;
    }
    ///////row
    if(Mstarted == true){
      ///////init char "<"
      if(aChar == 0x3C )
      {
        Rstarted = true;
        Rended = false; 
        RserialIn=0;
      }
      //   else if(aChar == '>')
      else if(aChar == 0x3E )
      {
        Rended = true;
        RserialIn=0;
      }
      ///// boxes values 
      else if(aChar == 48 || aChar == 49)
      {
        YuanBuffer[RserialIn] = aChar; 
        /*        Serial.print(RserialIn);
         Serial.print(":"); 
         Serial.println(aChar);
         */
        RserialIn++;
      }
      if(Rstarted && Rended)
      {
        delay(10);
        // We got a whole row
        for(int i=0;i<BOXNUM;i++){
          if(YuanBuffer[i] != 0) shotbuf[i][CserialIn%NUMSLICES]=YuanBuffer[i]-'0';
          else shotbuf[i][CserialIn%NUMSLICES]=0;
         //cleanup
          YuanBuffer[i]=0x00;
        }
        //        RserialIn = 0;
        Rstarted = false;
        Rended = false;
        CserialIn++;
      }
      //@ end matrix
      if(aChar == 0x7D){
        Mended = true;
        for(int i=0;i<NUMSLICES;i++){  
          Serial.print("i="); 
          Serial.print(i); 
          Serial.print(":");
          for(int j=0;j<BOXNUM;j++){
            ss[j] = shotbuf[j][i];
            Serial.print(ss[j]); 
          }
          Serial.println(";");
          shot(ss);
        }
        //        }
        CserialIn=0;
        Mstarted= false;
        Mended = false;
        for(int i=0;i<NUMSLICES;i++){  
          for(int j=0;j<BOXNUM;j++){
            shotbuf[j][i]=0;          
          }
        }
        break;     
      } 
    }
    //Serial.flush();
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

// on and off correnspond to 0 and F for MAX7313 - because of pullup R 
// these functions takes care of all the logging and book keeping 
// do no take into account Pauses needed between one shot and the other
//consider them private not to be used directly

void shotremapped(int i, int j){
  if (j==0) shotOff(remap(i));
  else if(j==1) shotFire(remap(i));
}

void shotpriv(int i, int j){
  if (j==0) shotOff(i);
  else if(j==1) shotFire(i);
}

void shotFire(int i){    //i from 1 to 18 !!! 
  //Serial.println(i);

  if ((i % 2)>0){ //numeri dispari
    if(i>100){
      int j=i-100;
      //Serial.println(j);
      LEDSstatusMax1[(j-1)/2] = (LEDSstatusMax1[(j-1)/2]  & 0xF0) +0x0F; //1,3,5,7,9... // &0xF0 masks higher bit 
      postino(max1, (0x10+(j-1)/2), LEDSstatusMax1[(j-1)/2]);
    }
    else  {
      //Serial.println(i);
      LEDSstatusMax2[(i-1)/2] = (LEDSstatusMax2[(i-1)/2]  & 0xF0) +0x0F; //1,3,5,7,9... // &0xF0 masks higher bit 
      //OK senza remap//
      postino(max2, (0x10+(i-1)/2), LEDSstatusMax2[(i-1)/2]); 
    }

  }
  else { //numeri pari 
    // postino(max2, (0x10+(i-1)/2), ((0x00 & 0x0F)+0xF0)); 
    if(i>100){
      int j =i-100;
      //Serial.println(j);
      LEDSstatusMax1[(j-1)/2] = (LEDSstatusMax1[(j-1)/2]  & 0x0F) +0xF0;//0,2,4,6,8... // &0x0F masks lower bit
      postino(max1, (0x10+(j-1)/2), LEDSstatusMax1[(j-1)/2]);
    }
    else{
      //Serial.println(i);
      LEDSstatusMax2[(i-1)/2] = (LEDSstatusMax2[(i-1)/2]  & 0x0F) +0xF0;//0,2,4,6,8... // &0x0F masks lower bit
      //ok senza remap//  
      postino(max2, (0x10+(i-1)/2), LEDSstatusMax2[(i-1)/2]);
    }
  }
}

void shotOff(int i){  
  if ((i % 2)>0){ //numeri dispari
    if(i>100){
      int j =i-100;
      LEDSstatusMax1[(j-1)/2] = (LEDSstatusMax1[(j-1)/2]  | 0x0F ) -0x0F; // |0x0F masks higher bit
      postino(max1, (0x10+(j-1)/2), LEDSstatusMax1[(j-1)/2]);
    }
    else{
      LEDSstatusMax2[(i-1)/2] = (LEDSstatusMax2[(i-1)/2]  | 0x0F ) -0x0F; // |0x0F masks higher bit
      postino(max2, (0x10+(i-1)/2), LEDSstatusMax2[(i-1)/2]); //1,3,5,7,9... 
      //postino(max2, (0x10+(i-1)/2),0x00);//0,2,4,6,8... 
    }
  }
  else { //numeri pari e 0
    if(i>100){
      int j =i-100;
      LEDSstatusMax1[(j-1)/2] = (LEDSstatusMax1[(j-1)/2]   | 0xF0) -0xF0; // |0xF0 masks lower bit
      postino(max1, (0x10+(j-1)/2),LEDSstatusMax1[(j-1)/2]);
    }
    else{
      LEDSstatusMax2[(i-1)/2] = (LEDSstatusMax2[(i-1)/2]   | 0xF0) -0xF0; // |0xF0 masks lower bit
      postino(max2, (0x10+(i-1)/2),LEDSstatusMax2[(i-1)/2]);//0,2,4,6,8... 
      // postino(max2, (0x10+(i-1)/2),0x00);//0,2,4,6,8... 
    }
  }
}

///////////////////////////////
// these is the public function 
// array contains 0=off 1=on
void shot(int *array){
  for (int j=0; j<BOXNUM; j++) {
    shotremapped(j+1,array[j]);
  }
  delay(Tshot);
  for (int j=0; j<BOXNUM; j++) {
    shotremapped(j+1,0);
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

  //MAX2
  else if (i>12 && i<=18) j=i-12+100; // + numero pari non cambia parita'
  else j=i;
  return j;
}


































