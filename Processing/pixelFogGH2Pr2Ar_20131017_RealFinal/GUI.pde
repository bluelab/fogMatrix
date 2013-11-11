//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
import controlP5.*;
ControlP5 controlP5;
boolean getObject = false;

void setupGUI(){
  
  controlP5 = new ControlP5(this);
  controlP5.setControlFont(font03);
  controlP5.setColorBackground(color(0));
  controlP5.setColorActive(color(255,0,0));
  controlP5.setColorForeground(color(100));
  controlP5.setColorLabel(color(255));
  /////////////////////////////////////// min,max, ,  x,  y,//////////
  Slider s1 = controlP5.addSlider("S01", 0, 180, 0, 10, 132, 100, 10); 
  s1.setId(1);
  
  Slider s2 = controlP5.addSlider("S02", 0, 180, 0, 10, 144, 100, 10); 
  s2.setId(2);
  Slider s3 = controlP5.addSlider("S03", 0, 180, 0, 10, 156, 100, 10); 
  s3.setId(3);
  Slider s4 = controlP5.addSlider("S04", 0, 180, 0, 10, 168, 100, 10); 
  s4.setId(4);
  Slider s5 = controlP5.addSlider("S05", 0, 180, 0, 10, 180, 100, 10); 
  s5.setId(5);
  Slider s6 = controlP5.addSlider("S06", 0, 180, 0, 10, 192, 100, 10); 
  s6.setId(6);
  Slider s7 = controlP5.addSlider("S07", 0, 180, 0, 10, 204, 100, 10); 
  s7.setId(7);
  Slider s8 = controlP5.addSlider("S08", 0, 180, 0, 10, 216, 100, 10); 
  s8.setId(8);
  Slider s9 = controlP5.addSlider("S09", 0, 180, 0, 10, 228, 100, 10); 
  s9.setId(9);
  Slider s10 = controlP5.addSlider("S10", 0, 180, 0, 10, 240, 100, 10); 
  s10.setId(10);
  Slider s11 = controlP5.addSlider("S11", 0, 180, 0, 10, 252, 100, 10); 
  s11.setId(11);
  Slider s12 = controlP5.addSlider("S12", 0, 180, 0, 10, 264, 100, 10); 
  s12.setId(12);
  Slider s13 = controlP5.addSlider("S13", 0, 180, 0, 10, 276, 100, 10); 
  s13.setId(13);
  Slider s14 = controlP5.addSlider("S14", 0, 180, 0, 10, 288, 100, 10); 
  s14.setId(14);
  Slider s15 = controlP5.addSlider("S15", 0, 180, 0, 10, 300, 100, 10); 
  s15.setId(15);
  Slider s16 = controlP5.addSlider("S16", 0, 180, 0, 10, 312, 100, 10); 
  s16.setId(16);
  Slider s17 = controlP5.addSlider("S17", 0, 180, 0, 10, 324, 100, 10); 
  s17.setId(17);
  Slider s18 = controlP5.addSlider("S18", 0, 180, 0, 10, 336, 100, 10); 
  s18.setId(18);
  
  
  controlP5.addButton("RESET", 0, 10, 93, 42, 20);
  
  controlP5.addButton("s03", 0, 10, 365, 26, 20);
  controlP5.addButton("s06", 0, 39, 365, 26, 20); 
  controlP5.addButton("s09", 0, 68, 365, 26, 20); 
  controlP5.addButton("s12", 0, 97, 365, 26, 20); 
  controlP5.addButton("s15", 0, 126, 365, 26, 20); 
  controlP5.addButton("s18", 0, 155, 365, 26, 20); 
  
  controlP5.addButton("s02", 0, 10, 388, 26, 20);
  controlP5.addButton("s05", 0, 39, 388, 26, 20); 
  controlP5.addButton("s08", 0, 68, 388, 26, 20); 
  controlP5.addButton("s11", 0, 97, 388, 26, 20); 
  controlP5.addButton("s14", 0, 126, 388, 26, 20); 
  controlP5.addButton("s17", 0, 155, 388, 26, 20);
 
  controlP5.addButton("s01", 0, 10, 411, 26, 20); 
  controlP5.addButton("s04", 0, 39, 411, 26, 20); 
  controlP5.addButton("s07", 0, 68, 411, 26, 20); 
  controlP5.addButton("s10", 0, 97, 411, 26, 20); 
  controlP5.addButton("s13", 0, 126, 411, 26, 20); 
  controlP5.addButton("s16", 0, 155, 411, 26, 20);  
  
  
  controlP5.setAutoDraw(false);  
}

void controlEvent(ControlEvent theEvent) {

  switch(theEvent.controller().id()){
    
    case(1):
    S01 = (int)theEvent.controller().value();
    break;
    case(2):
    S02 = (int)theEvent.controller().value();
    break;
    case(3):
    S03 = (int)theEvent.controller().value();
    break;
    case(4):
    S04 = (int)theEvent.controller().value();
    break;
    case(5):
    S05 = (int)theEvent.controller().value();
    break;
    case(6):
    S06 = (int)theEvent.controller().value();
    break;
    case(7):
    S07 = (int)theEvent.controller().value();
    break;
    case(8):
    S08 = (int)theEvent.controller().value();
    break;
    case(9):
    S09 = (int)theEvent.controller().value();
    break;
    case(10):
    S10 = (int)theEvent.controller().value();
    break;
    case(11):
    S11 = (int)theEvent.controller().value();
    break;
    case(12):
    S12 = (int)theEvent.controller().value();
    break;
    case(13):
    S13 = (int)theEvent.controller().value();
    break;
    case(14):
    S14 = (int)theEvent.controller().value();
    break;
    case(15):
    S15 = (int)theEvent.controller().value();
    break;
    case(16):
    S16 = (int)theEvent.controller().value();
    break;
    case(17):
    S17 = (int)theEvent.controller().value();
    break;
    case(18):
    S18 = (int)theEvent.controller().value();
    break;     
  }  
} 

void RESET(){
  
  //setup();
  getCamCenter();  
}

void s01(){
  
     
}

void s02(){
  
   
}

void s03(){
  
  
}

void s04(){
  
   
}

void s05(){
  
   
}

void s06(){
  
  
}

void s07(){
  
  
}

void s08(){
  
   
}

void s09(){
  
 
}

void s10(){
  
   
}

void s11(){
  
   
}

void s12(){
  
   
}

void s13(){
  
   
}

void s14(){
  
   
}

void s15(){
  
   
}

void s16(){
  
   
}

void s17(){
  
   
}

void s18(){
  
   
}

void gui(){
   
  if(mouseX < 200 && mouseY < 200) cam.setActive(false);
  if(mouseX > 200 && mouseY > 200) cam.setActive(true);
   
  cam.beginHUD();
  camera();
  controlP5.draw(); 
  //textMode(SCREEN);
  fill(0);
  stroke(0);
  textFont(font02, 25);
  text("FOGMATRIX", 10, 30);

  fill(255);
  stroke(255);
  textFont(font02, 10);
  text("V0.1", 160, 30);
  
  fill(0);
  stroke(0);
  textFont(font02, 10);
  text("copyright", 10, 50);
  
  fill(255);
  stroke(255);
  textFont(font02, 15);
  text("SINLAB", 60, 50);
  
  fill(0);
  stroke(0);
  textFont(font02, 10);
  text("Prof. Jeffrey Huang", 10, 62);
  text("Shih-Yuan Wang, Alex Barchiesi", 10, 74);
  
  text("------------------------------------------------", 10, 86); 
  text("SERVO MOTOR-------------------------", 10, 126);
  text("LOUDSPEAKER-------------------------", 10, 360);
  
   
  cam.endHUD(); 
 }
 /////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
