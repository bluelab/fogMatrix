//Alex Barchiesi for fog rings project 
// based partially on the code for max7313 by  eric toering
// Using the Wire library (created by Nicholas Zambetti)
// On the Arduino board, Analog In 4 is SDA, Analog In 5 is SCL
//s 3 row= 
//{<111111000000000000><000000110000000000><000000000000110000><110000000000000000><000000111111000000><000000000000000011><000011000000000000><000000000011000000><000000000000111111>}
//i 3 row= 
//{<001100000000000000><000000001100000000><000000000000001100><001100000000000000><000000001100000000><000000000000001100><001100000000000000><000000001100000000><000000000000001100>}
//n 3 row= 
//{<000000000000100001><1100010000000000><000000110001000000><000000000000101001><1010010000000000><000000100101000000><000000000000100101><1000110000000000><000000100011000000>}
//l 3 row= 
//{<110000000000000000><000000110000000000><000000000000110000><110000000000000000><000000110000000000><000000000000110000><110000000000000000><000000111111000000><000000000000111111>}
//a 3 row= 
//{<000100000000000000><000000000100000000><000000000000001010><00101000000000000><0000000100010000000><000000000000010001><111111000000000000><000000111111000000><000000000000100001>}
//b 3 row= 
//{<111111000000000000><000000110011000000><000000000000110011><110011000000000000><000000111111000000><000000000000110011><110011000000000000><000000110011000000><000000000000111111>}


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
#define NUMSLICES  9// number of "slices" = height //8 = max independent from serial buffer
#define DEBUG 0 // if 0 activate serial write

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
int debugT=00, Tdelay=250; //set debugT to 0 when not debuggin
//ok
//int Tcharge=300, Tshot=10; 
//
int Tcharge=100, Tshot=10; 

int ss[BOXNUM];
int shotbuf[BOXNUM][NUMSLICES];
int S[NUMSLICES][BOXNUM]={
  {
    1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0                    }
  ,{
    0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0                    }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0                    }
  ,{
    1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0                    }
  ,{
    0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0                    }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1                    }
  ,{
    0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0                    }
  ,{
    0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0                    }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1                    }
};
int I[NUMSLICES][BOXNUM]={
  {
    0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0                }
  ,{
    0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0                }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0                }
  ,{
    0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0                }
  ,{
    0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0                }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0                }
  ,{
    0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0                }
  ,{
    0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0                }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0                }
};
int N[NUMSLICES][BOXNUM]={
  {
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1              }
  ,{
    1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0                  }
  ,{
    0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0              }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1              }
  ,{
    1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0                  }
  ,{
    0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,0              }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1              }
  ,{
    1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0                  }
  ,{
    0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0              }
  ,
};
int L[NUMSLICES][BOXNUM]={
  {
    1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0            }
  ,{
    0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0            }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0            }
  ,{
    1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0            }
  ,{
    0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0            }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0            }
  ,{
    1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0            }
  ,{
    0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0            }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1            }
};
int A[NUMSLICES][BOXNUM]={
  {
    0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0          }
  ,{
    0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0          }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0          }
  ,{
    0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0            }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0          }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1          }
  ,{
    1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0          }
  ,{
    0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0          }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1          }
};
int B[NUMSLICES][BOXNUM]={
  {
    1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0      }
  ,{
    0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0      }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1      }
  ,{
    1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0      }
  ,{
    0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0      }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1      }
  ,{
    1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0      }
  ,{
    0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0      }
  ,{
    0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1      }
};

////
boolean LED13=false;
////

/////////////////////////////////////////////////////////////
//servo
/////////////////////////////////////////////////////////////
#define SD21Address  0x61 //The address of the SD21
int SERVO[10] = {
  0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47};
/*int test[4][NUMSLICES] = {{
 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47},{
 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47},{
 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47},{
 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47}};
 */

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
  //flag to processing code to start transmission
  Serial.print("!");
  ///////

  // sinlab();
  // delay(1000);
  // int input = Serial.read();  // read serial 
  int T=150; //ms
  ////receiving from processing  
  bool Rstarted=false, Rended=false, Mstarted=false, Mended=false;
  int CserialIn=0, RserialIn=0;

  //s = {<111111><110000><110000><110000><111111><000011><000011><000011><111111>}
  //i =
  while(Serial.available() > 0){
    // Serial.print(0x66);
    ////////////////////////from processing use something like:
    //    myPort.write("{"); //ascii 0x7B
    //    myPort.write("<"); //ascii 0x3C
    //    myPort.write(val);
    //    myPort.write(">"); //ascii 0x3E
    //    myPort.write("}"); //ascii 0x7D 
    ////////////////////////  to send the array

    byte aChar = Serial.read();
    //////init char "{"
    if(aChar == 0x7B){
      Mstarted = true;
      Mended = false;
      if(DEBUG)    Serial.println("Mstarted");

    }
    ///////row
    if(Mstarted == true){
      ///////init char "<"
      if(aChar == 0x3C )
      {
        Rstarted = true;
        Rended = false; 
        RserialIn=0;
        if(DEBUG)        Serial.println("<");
      }
      //   else if(aChar == '>')
      else if(aChar == 0x3E )
      {
        Rended = true;
        RserialIn=0;
        if(DEBUG)    Serial.println(">");
      }
      ///// boxes values 
      else if(aChar == 48 || aChar == 49)
      {
        YuanBuffer[RserialIn] = aChar; 
        if(DEBUG){    
          Serial.print(RserialIn);
          Serial.print(":"); 
          Serial.println(aChar);
        }
        RserialIn++;
      }
      else{
        if(DEBUG)       Serial.println("else char");
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
        if(DEBUG)   Serial.println("Rstarted && Rended");
      }
      //@ end matrix "}"
      if(aChar == 0x7D){
        Mended = true;
        for(int i=0;i<NUMSLICES;i++){  
          if(DEBUG)  {  
            Serial.print("i="); 
            Serial.print(i); 
            Serial.print(":");
          }
          for(int j=0;j<BOXNUM;j++){
            ss[j] = shotbuf[j][i];
            if(DEBUG)    Serial.print(ss[j]); 
          }
          if(DEBUG)    Serial.println(";");
          //for(int k = 0; k < 8; k++) shot(ss);  
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
        if(DEBUG)      Serial.println("end");
        //delay(1000);
        break;     
      }
      //     
    }  

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
  delay(Tcharge); //dovrebbe essere Tcharge
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
  for(int i=0;i<NUMSLICES;i++){
    shot(S[i]);
  }
};

void i(){
  for(int i=0;i<NUMSLICES;i++){
    shot(I[i]);
  }
};

void n(){
  for(int i=0;i<NUMSLICES;i++){
    shot(N[i]);
  }
};

void l(){
  for(int i=0;i<NUMSLICES;i++){
    shot(L[i]);
  }
};

void a(){
  for(int i=0;i<NUMSLICES;i++){
    shot(A[i]);
  }
};

void b(){
  for(int i=0;i<NUMSLICES;i++){
    shot(B[i]);
  }
};

void sinlab(){
  s();
  delay(2000);
  i();
  delay(2000);
  n();
  delay(2000);
  l();
  delay(2000);
  a();
  delay(2000);
  b();
  delay(2000);
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
















































