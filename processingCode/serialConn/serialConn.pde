/*
 *Works with Arduino sketch 'pdxMap'
 *and arduino 8 LED sheild
 */

import processing.serial.*;

Serial myPort;
int a = 200;
int part=20;
boolean[] buttonOn= new boolean[part];

void setup() {  
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[1], 9600);
  myPort.buffer(1);
  size(screenWidth, 200);
  background(51);
  // noLoop();  // Makes draw() only run once
  for (int i=0; i<part ; i++) {
    buttonOn[i]=false;
  }
}

void draw() {
  for (int i=0;i<part;i++) {
    if (i<9) {
      if (buttonOn[i]) {
        fill(a, 0, 0);
      }
      else { 
        fill(0, a, a);
        rect(width/part*i, 0, width/part, height);
        fill(50);
        text("box"+(i+1), (width/part*(i+1)+width/part*i)*.5-10, height*.5);
      }
    }
    else {
      fill(a/2, a/2, a/2);
      rect(width/part*i, 0, width/part, height);
    }
    if (i==16) {        
      fill(50);
      text("all", (width/part*(i+1)+width/part*i)*.5 -10, height*.5);
    }
    if (i==18) {        
      fill(50);
      text("ramp", (width/part*(i+1)+width/part*i)*.5-10, height*.5);
    }
    if (i==19) {        
      fill(50);
      text("cuspid", (width/part*(i+1)+width/part*i)*.5-15, height*.5);
    }
    if (i==12) {        
      fill(50);
      text("selected", (width/part*(i+1)+width/part*i)*.5-20, height*.5);
    }
  }
}



void mousePressed() {
  println("Coordinates: " + mouseX +"," + mouseY);
  mouseAction();
}

void mouseAction() {
  for (int i=0;i<part;i++) {
    if (mouseX>width/part*i && mouseX<width/part*(i+1)) {
      buttonOn[i]=!buttonOn[i];
      println("button: "+(i+1)+ "; i="+i);
      // button "selected"
      if (i==12) {
        for (int j=0;j<9;j++) {
          if (buttonOn[j])
            myPort.write(j+1);
        }
      }  
      // std button 
      else
        myPort.write(i+1);    
/////////GUY no more port write 
      //change colors to the buttons   
      if (i<9) {
        if (buttonOn[i]) {
          fill(a, 0, 0, 50); //grigetto
        }
        else fill(0, a, a);
        rect(width/part*i, 0, width/part, height);
      }
      else {
        fill(0, a/2, 0);
        rect(width/part*i, 0, width/part, height);
        fill(a/2, a/2, a/2);
        rect(width/part*i, 0, width/part, height);
        //       fill(255, 255, 255);
        //       text(i, (width/part*(i+1)-width/part*i)*.5, height*.5);
      }
    }
  }
}

