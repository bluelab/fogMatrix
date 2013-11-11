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
      float y = float(fields[1])*(-1);
      float z = float(fields[2]); 
      int index = int(fields[3]);
      float time = float(fields[4]);
      camCenter = new PVector(float(fields[5]),float(fields[6])*(-1),float(fields[7]));
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
     if(getPt.getIndex() == 48){
       
       fill(255,0,100);
       noStroke();
       textFont(font01, 4);
       text(i, getPt.getLocation().x, getPt.getLocation().y,getPt.getLocation().z);       
     }else if(getPt.getIndex() == 49){
      
       fill(0,0,255);
       noStroke();
       textFont(font01, 4);
       text(i, getPt.getLocation().x, getPt.getLocation().y,getPt.getLocation().z);
       pushMatrix(); 
       translate(getPt.getLocation().x, getPt.getLocation().y,getPt.getLocation().z); 
       fill(0,0,255);
       stroke(0);
       strokeWeight(0.5);
       box(10); 
       popMatrix();     
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

int countCam = 0;
void getCamCenter(){
 
 if(camCenter != null){ 
   cam.lookAt(camCenter.x, camCenter.y, camCenter.z,0);
   if(countCam == 0){
    cam.rotateX(-1);
    cam.rotateY(0.5);
    countCam = -1;
   }
   
 }
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
