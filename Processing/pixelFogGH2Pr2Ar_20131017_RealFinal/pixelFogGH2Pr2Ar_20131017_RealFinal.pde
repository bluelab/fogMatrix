/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//pixelFog project by Shih-Yuan Wang
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
import processing.serial.*;
import hypermedia.net.*; 
import peasy.*; 
PeasyCam cam;
UDP udp;  
String getData;
ArrayList pointCloud;
ArrayList collectLayer;
PFont font01,font02,font03;
int S01 = 0; int S02 = 0; int S03 = 0; int S04 = 0; int S05 = 0; int S06 = 0; int S07 = 0;
int S08 = 0; int S09 = 0; int S10 = 0; int S11 = 0; int S12 = 0; int S13 = 0; int S14 = 0;
int S15 = 0; int S16 = 0; int S17 = 0; int S18 = 0;
Serial port;


byte arrayBoxes[];

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  size(1000,600,P3D);
  font01 = loadFont("Arial-BoldMT-50.vlw");
  font02 = loadFont("Arial-BoldMT-25.vlw");
  font03 = loadFont("Arial-BoldMT-10.vlw");
  //sketch to the desired serial port.
  println(Serial.list());
  port = new Serial(this, Serial.list()[0], 115200);
  port.buffer(1); 
  //frameRate(30);
  cam = new PeasyCam(this, 500);
  cam.lookAt(0, 0, 0);
  udp = new UDP(this, 6001);
  udp.listen( true );
  setupGUI();
}


void draw() {
  
 background(190);
 displayAxis();
 update();
 //sendToArduino();
 Arduino2ProcessingSpeaker();
 gui(); 
}


