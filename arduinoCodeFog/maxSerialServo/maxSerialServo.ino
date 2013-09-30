//NB 
//SERVE V status nel quale deve esserci sempre lo stato della macchina ? 
//ogni fire in realta' ha una pausa quindi stato e' sempre 0
//2DO keep track of addresses in glob vars e usare OR AND

// alloff necessita di una pausa 
// serve F che prenda vettore di numeri e spari tutti i num 
// capire che cazzo non funzionava oggi
//2 DO / loop on board config 

//Alex Barchiesi for fog rings project 

//2DO / address matrix loop board - led 

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
int LEDSarray[16];//numers of LED arrays = controller boards

int  LEDSON[16]; //array for all on 
int  LEDSOFF[16]; //array for all off 
int  LEDSLETTER[16]; //array for all off 
  
//front 8 6 4 
int LEDSFRONT[16];

//back 1 2 3 5 7 9
int LEDSBACK[16];

//2DO transform into array for multi max 
byte LEDSstatus[16]; //current machine status first 8 values correnspond to the 16 fog rings - second  8 to the motors
int BOXNUM=16; //number of boxes 16

//byte chipdata = 0;

///debug vars
int debugT=00, Tdelay=50; //set debugT to 0 when not debuggin
int Tcharge=1000, Tshot=100; 
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

  /*  small routine for finding out how the addresss pins are connected
   for (int i = 0; i < 255; i++){
   max1 = i;
   
   postino(max1, 0xf, 0x10);                  
   postino(max1, 0x6, 0x00);                  
   postino(max1, 0x7, 0x00);                  
   postino(max1, 0x2, 0xff);                  
   postino(max1, 0x3, 0xff);
   postino(max1, 0xe, 0xff);            
   postino(max1, 0x10, 0x00); 
   postino(max1, 0x11, 0x00); 
   postino(max1, 0x12, 0x00); 
   postino(max1, 0x13, 0x00); 
   postino(max1, 0x14, 0x00); 
   postino(max1, 0x15, 0x00); 
   postino(max1, 0x16, 0x00); 
   postino(max1, 0x17, 0x00); 
   Serial.println(i, DEC);
   delay (5);
   }
   
   for (int i = 0; i < 255; i++){
   max1 = i;
   postino(max1, 0xf, 0x10);                  
   postino(max1, 0x6, 0x00);                  
   postino(max1, 0x7, 0x00);                  
   postino(max1, 0x2, 0xff);                   
   postino(max1, 0x3, 0xff);
   postino(max1, 0xe, 0xff);                  
   postino(max1, 0x10, 0xff); 
   postino(max1, 0x11, 0xff);
   postino(max1, 0x12, 0xff); 
   postino(max1, 0x13, 0xff); 
   postino(max1, 0x14, 0xff); 
   postino(max1, 0x15, 0xff); 
   postino(max1, 0x16, 0xff); 
   postino(max1, 0x17, 0xff); 
   Serial.println(i, DEC);
   delay (5);
   }
   
   */
  allOff();
  delay(100);
  ///////
  //shotAll();  
  int input = Serial.read();  // read serial 
  int T=150; //ms
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
    shotBack();  
    LED13!=LED13; // set the LED  
    break;
  case 5:         
    shotFront();
    LED13!=LED13; // set the LED
    break;
  }

  /*   ////intesity loop for future devel (ring different power)
   
   for (int x = 0; x< 5; x++){
   while (intensita > 0){
   
   for (int i = 0x10; i < 0x18; i++){
   postino(max1, i, intensita); 
   postino(max2, i, intensita);
   }
   
   delay(40+debugT);
   
   intensita -= 0x11;
   
   }
   
   while (intensita  < 0xff){
   for (int i = 0x10; i < 0x18; i++){
   postino(max1, i, intensita); 
   postino(max2, i, intensita); 
   }
   
   delay(60+debugT);
   
   intensita += 0x11; 
   }  
   
   } 
   
   
   for (int i = BOXNUM; i > 0; i--){
   if ((i % 2)>0){ 
   postino(max2, (0x10+(i / 2)), 0x0F); 
   }
   else { 
   postino(max2, (0x10+(i / 2)), 0xF0);
   }
   delay(50+debugT);
   postino(max2, (0x10+(i / 2)), 0xFF);
   }
   
   for (int i = 0; i < BOXNUM; i++){
   if ((i % 2)>0){ 
   postino(max1, (0x10+(i / 2)), 0x0F); 
   delay(debugT);    
   Serial.println(i);
   }
   else { 
   postino(max1, (0x10+(i / 2)), 0xF0);
   delay(debugT); 
   Serial.println(i);
   }
   delay(80+debugT);
   postino(max1, (0x10+(i / 2)), 0xFF);
   }
   /////
   
   */
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

void shotFire(int i){  
  if ((i % 2)>0){ //numeri dispari
    LEDSstatus[(i-1)/2] = (LEDSstatus[(i-1)/2]  & 0xF0) +0x0F; //1,3,5,7,9... // &0xF0 masks higher bit 
    postino(max2, (0x10+(i-1)/2), LEDSstatus[(i-1)/2]); 
    //  postino(max2, (0x10+(i-1)/2), ((0x00 & 0xF0)+0x0F)); 
  }
  else { //numeri pari e 0
    LEDSstatus[(i-1)/2] = (LEDSstatus[(i-1)/2]  & 0x0F) +0xF0;//0,2,4,6,8... // &0x0F masks lower bit
    postino(max2, (0x10+(i-1)/2), LEDSstatus[(i-1)/2]);
    // postino(max2, (0x10+(i-1)/2), ((0x00 & 0x0F)+0xF0)); 
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
    shotpriv(j,array[j]);
  }
  delay(Tshot);
  for (int j=0; j<BOXNUM; j++) {
    shotpriv(j,0);
  }
  delay(Tshot); //dovrebbe essere Tcharge
}

void shotSingle(int i){
  int SINGLELED[BOXNUM];
  for(int h=0; h<BOXNUM; h++){  
    //    SINGLELED[h]=LEDSOFF[h];
    SINGLELED[h]=0;
  }
  SINGLELED[i]=1;
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

void shotFront(){ 
  shot(LEDSFRONT);
}
void shotBack(){ 
  shot(LEDSBACK);
}

//

void s(){
  //back 1 2 3 5 7 9
  //LEDSBACK
  shot(LEDSBACK);
  for(int h=0;h<3;h++) shotSingle(1);
  shot(LEDSBACK);
  for(int h=0;h<3;h++) shotSingle(9);
  shot(LEDSBACK);
};

void i(){
  for(int i=0;i<BOXNUM;i++){
    LEDSLETTER[i]=0;
  }
  LEDSLETTER[3]=1;
  LEDSLETTER[5]=1;
  for(int h=0;h<7;h++)  shot(LEDSLETTER);
};

void n(){
  for(int i=0;i<BOXNUM;i++){
    LEDSLETTER[i]=0;
  }
  LEDSLETTER[1]=1;
  LEDSLETTER[9]=1;
  shot(LEDSLETTER);
  LEDSLETTER[7]=1;
  shot(LEDSLETTER);
  LEDSLETTER[7]=0;
  LEDSLETTER[5]=1;
  shot(LEDSLETTER);
  LEDSLETTER[5]=0;
  LEDSLETTER[3]=1;
  shot(LEDSLETTER);
  LEDSLETTER[3]=0;
  LEDSLETTER[2]=1;
  shot(LEDSLETTER);
  LEDSLETTER[2]=0;
  LEDSLETTER[1]=1;
  shot(LEDSLETTER);
};

void l(){
  for(int h=0;h<6;h++) shotSingle(9);
  shot(LEDSBACK);
};

void a(){
  for(int i=0;i<BOXNUM;i++){
    LEDSLETTER[i]=0;
  }
  LEDSLETTER[1]=1;
  LEDSLETTER[9]=1;
  shot(LEDSBACK);  
  shot(LEDSLETTER);
  shot(LEDSBACK);  
  shot(LEDSLETTER);
  shot(LEDSLETTER);
  shot(LEDSLETTER);
  shot(LEDSLETTER);
};

void b(){
  for(int i=0;i<BOXNUM;i++){
    LEDSLETTER[i]=0;
  }
  LEDSLETTER[1]=1;
  LEDSLETTER[9]=1;
  shot(LEDSBACK);  
  shot(LEDSLETTER);
  shot(LEDSLETTER);
  shot(LEDSBACK);  
  shot(LEDSLETTER);
  shot(LEDSLETTER);
  shot(LEDSBACK); 
};
/*
 void x{
 };
 void e{
 };
 
 void y{
 };
 void u{
 };
 
 //in fila
 void sinlab(){
 
 //S
 for(int i=1;i<6;i++){
 postino(max2, (0x10+(i-1)/2), 0xFF); //
 }
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 for(int c=0;c<5;c++){
 shotSingle(6);
 delay(5*Tshot);
 }
 allOff();
 delay(Tshot);
 
 for(int i=1;i<6;i++){
 postino(max2, (0x10+(i-1)/2), 0xFF); //
 }
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 
 for(int c=0;c<5;c++){
 shotSingle(1);
 delay(5*Tshot);
 }
 allOff();
 delay(Tshot);
 
 for(int i=1;i<6;i++){
 postino(max2, (0x10+(i-1)/2), 0xFF); //
 }
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 
 
 //I
 delay(10*Tcharge);
 for(int c=0;c<7;c++){
 shotSingle(5);
 delay(5*Tshot);
 }
 allOff();
 delay(10*Tcharge);
 
 //N
 //167
 shotFire(1);
 postino(max2, (0x10), 0x0F);//1
 postino(max2, (0x10+2), 0xF0);//6
 postino(max2, (0x10+3), 0x0F);//7
 
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 //167
 shotFire(1);
 postino(max2, (0x10), 0x0F);//1
 postino(max2, (0x10+2), 0xF0);//6
 postino(max2, (0x10+3), 0x0F);//7
 
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 
 //157
 postino(max2, (0x10), 0x0F);//1
 postino(max2, (0x10+2), 0x0F);//5
 postino(max2, (0x10+3), 0x0F);//7
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 //157
 postino(max2, (0x10), 0x0F);//1
 postino(max2, (0x10+2), 0x0F);//5
 postino(max2, (0x10+3), 0x0F);//7
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 
 
 
 //147
 postino(max2, (0x10), 0x0F);//1
 postino(max2, (0x10+1), 0xF0);//4
 postino(max2, (0x10+3), 0x0F);//7
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 //147
 postino(max2, (0x10), 0x0F);//1
 postino(max2, (0x10+1), 0xF0);//4
 postino(max2, (0x10+3), 0x0F);//7
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 
 //137
 postino(max2, (0x10), 0x0F);//1
 postino(max2, (0x10+1), 0x0F);//3
 postino(max2, (0x10+3), 0xF0);//7
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 //137
 postino(max2, (0x10), 0x0F);//1
 postino(max2, (0x10+1), 0x0F);//3
 postino(max2, (0x10+3), 0xF0);//7
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 
 
 //127
 postino(max2, (0x10), 0xFF);//1
 postino(max2, (0x10+3), 0x0F);//7
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 //127
 postino(max2, (0x10), 0xFF);//1
 postino(max2, (0x10+3), 0x0F);//7
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 
 
 //L
 delay(10*Tcharge);
 for(int c=0;c<7;c++){
 shotSingle(6);
 delay(5*Tshot);  
 }
 allOff();
 delay(Tshot);
 
 for(int i=1;i<7;i++){
 postino(max2, (0x10+(i-1)/2), 0xFF); //
 }
 delay(5*Tshot);  
 allOff();
 
 
 
 //A
 delay(10*Tcharge);
 shotSingle(5);
 delay(5*Tshot);
 
 shotFire(4);
 shotFire(6);
 delay(2*Tshot); 
 allOff();
 delay(2*Tshot); 
 
 shotFire(3);
 shotFire(7);
 delay(2*Tshot); 
 allOff();
 delay(2*Tshot); 
 
 
 postino(max2, (0x10+1), 0xFF);
 postino(max2, (0x10+2), 0xFF);
 delay(2*Tshot); 
 allOff();
 delay(2*Tshot); 
 
 shotFire(2);
 shotFire(8);
 delay(2*Tshot); 
 allOff();
 delay(2*Tshot); 
 
 
 shotFire(1);
 shotFire(9);
 delay(2*Tshot); 
 allOff();
 delay(2*Tshot); 
 
 
 //B
 delay(10*Tcharge);
 for(int i=1;i<4;i++){
 postino(max2, (0x10+(i-1)), 0xFF); //
 }
 delay(2*Tshot); 
 allOff();
 delay(2*Tshot); 
 
 for(int c=0;c<3;c++){
 shotFire(7);
 shotFire(1);
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 }
 
 
 for(int i=1;i<4;i++){
 postino(max2, (0x10+(i-1)), 0xFF); //
 }
 delay(2*Tshot); 
 allOff();
 delay(2*Tshot); 
 
 
 for(int c=0;c<3;c++){
 shotFire(7);
 shotFire(1);
 delay(2*Tshot);
 allOff();
 delay(2*Tshot);
 }
 
 
 for(int i=1;i<4;i++){
 postino(max2, (0x10+(i-1)), 0xFF); //
 }
 delay(2*Tshot); 
 allOff();
 delay(2*Tshot); 
 
 
 
 }
 
 
 
 
 */











































































