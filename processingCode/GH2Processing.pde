/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//pixelFog project by Yuan and Alex
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
import hypermedia.net.*; 
import processing.opengl.*;
import peasy.*; 
PeasyCam cam;
UDP udp;  
String getData;
ArrayList pointCloud;
PFont font01,font02;

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  size(1000,600,P3D);
  font01 = loadFont("Arial-BoldMT-50.vlw");
  font02 = loadFont("Arial-BoldMT-25.vlw");
  cam = new PeasyCam(this, 500);
  if(camCenter != null) getCamCenter();
  udp = new UDP(this, 6001);
  udp.listen( true );
  setupGUI();
}

void draw(){
  
 background(200);
 displayAxis();
 update();
 gui();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void receive( byte[] data, String ip, int port ){	// <-- extended handler
  String message = new String( data );   
  println(message);
  getData = message;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
PVector camCenter;
PVector axisCenter;

void getPoints(){
  
  pointCloud = new ArrayList();
  String [] getPts01 = new String[1];
  getPts01[0] = getData; 
  String [] getPts02 = new String[1]; 
  
  if(getPts01[0] != null){
    getPts02 = split(getPts01[0],'|');
  }

  for(int i = 0; i < getPts02.length-1; i++){
    if(getPts02[i] != null){
      String [] fields = split(getPts02[i],',');   
      float x = float(fields[0]);
      float y = float(fields[1]);
      float z = float(fields[2]); 
      int index = int(fields[3]);
      float time = float(fields[4]);
      camCenter = new PVector(float(fields[5]),float(fields[6]),float(fields[7]));
      PVector location = new PVector(x,y,z);
      Points ptLoc = new Points(location,index,time);
      pointCloud.add(ptLoc); 
      
      if(i == 0) axisCenter = new PVector(x,y,z);    
    }   
  }
}

void displayPoint(){
  Points getPt;
  for(int i = 0; i < pointCloud.size(); i++){
   
     getPt = (Points)pointCloud.get(i);
     getPt.display();
  }
}

void displayTextPts(){
  
  Points getPt;
  for(int i = 0; i < pointCloud.size(); i++){
   
     getPt = (Points)pointCloud.get(i);
     if(getPt.getIndex() == 0){
       
       fill(255,0,100);
       noStroke();
       textFont(font01, 6);
       text(i, getPt.getLocation().x, getPt.getLocation().y,getPt.getLocation().z);       
     }else if(getPt.getIndex() == 1){
      
       fill(0,0,255);
       noStroke();
       textFont(font01, 6);
       text(i, getPt.getLocation().x, getPt.getLocation().y,getPt.getLocation().z);       
     }         
  } 
}

void update(){
  
  getPoints();
  displayPoint();
  displayTextPts();  
}

void displayAxis(){
 
 if(axisCenter != null){
   //x
   stroke(255,0,0);
   strokeWeight(2);
   line(axisCenter.x,axisCenter.y,axisCenter.z,axisCenter.x+10,axisCenter.y,axisCenter.z);
   //y
   stroke(0,0,255);
   strokeWeight(2);
   line(axisCenter.x,axisCenter.y,axisCenter.z,axisCenter.x,axisCenter.y+10,axisCenter.z);
   //z
   stroke(0,255,0);
   strokeWeight(2);
   line(axisCenter.x,axisCenter.y,axisCenter.z,axisCenter.x,axisCenter.y,axisCenter.z+10); 
 } 
}

void getCamCenter(){
  
  cam.lookAt(camCenter.x, camCenter.y, camCenter.z, 0);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
import controlP5.*;
ControlP5 controlP5;
boolean getObject = false;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void setupGUI(){
  
  controlP5 = new ControlP5(this);
  controlP5.setColorBackground(color(100));
  controlP5.setColorActive(color(255));
  controlP5.setColorForeground(color(0));
  controlP5.setColorLabel(color(255));
  /////////////////////////////////////// min,max, ,  x,  y,//////////
  //Slider s1 = controlP5.addSlider("wid", 10, 100, 0, 10, 90, 108, 10); 
  //s1.setId(1);
  controlP5.addButton("RESET", 0, 10, 80, 30, 20); 
  controlP5.setAutoDraw(false);  
}

void RESET(){
  
  setup();   
}

void gui(){
   
  if(mouseX < 200 && mouseY < 200) cam.setActive(false);
  if(mouseX > 200 && mouseY > 200) cam.setActive(true);
   
  cam.beginHUD();
  camera();
  controlP5.draw(); 
  textMode(SCREEN);
  fill(0);
  stroke(0);
  textFont(font02, 25);
  text("FOGMATRIX", 10, 30);

  textFont(font02, 10);
  text("V0.1", 160, 30);
  
  fill(255);
  stroke(255);
  textFont(font02, 10);
  text("copyright  SINLAB", 10, 50);
  
  fill(255);
  stroke(255);
  textFont(font02, 10);
  text("Prof. Jeffrey Huang", 10, 60);
  text("Project by Shih-Yuan Wang and Alex Barchiesi", 10, 70);
   
  cam.endHUD(); 
 }
 
 class Points{
  
 //////////////////////////////////////////////////////////////////////////
 //PROPERTIES//////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////
 PVector location;
 int index;
 float time;
  
 ////////////////////////////////////////////////////////////////////////// 
 //CONSTRUCTOR/////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////
 Points(PVector location, int index, float time){
   
  this.location = location;
  this.index = index;
  this.time = time;
 }

 //////////////////////////////////////////////////////////////////////////
 //METHOD////////////////////////////////////////////////////////////////// 
 //////////////////////////////////////////////////////////////////////////
 void display(){
   
  if(index == 0){  
    stroke(255,255,255);
    strokeWeight(4);
    point(location.x,location.y,location.z);   
  }else if(index == 1){
    stroke(0,0,0);
    strokeWeight(4);
    point(location.x,location.y,location.z);   
  }   
 }
  
 PVector getLocation(){
   
  return location; 
 }
 
 int getIndex(){
   
  return index; 
 }
 
 float getTime(){
   
  return time; 
 }
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

