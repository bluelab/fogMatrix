/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
int buffer = 0;
void sendToArduino() {  

  if (buffer > 300) {

    port.write("z" + S01);
    port.write("x" + S02);
    port.write("c" + S03);
    port.write("v" + S04);
    port.write("b" + S05);
    port.write("n" + S06);
    //port.write("a" + S07);
    //port.write("s" + S08);

    buffer = 0;
  }

  buffer++;



  /////////////////////////////////////////////////
}

//102 -> 5 layer
//122 -> 6 layer
byte collectDataToArduino[] = new byte[202];
ArrayList<Byte> collectionData;
void Arduino2ProcessingSpeaker() {

  if (pointCloud.size() != 0) { 


    collectionData = new ArrayList<Byte>();
    int counter01 = 1;
    int counter02 = 0;
    for (int i = 0; i < pointCloud.size(); i++) {

      Points getPts = (Points)pointCloud.get(i);
      collectionData.add((byte)getPts.getIndex());
    }

    //add "><" in the list
    for (int j = 0; j < collectionData.size(); j++) {

      if (j == 0) collectionData.add(j, (byte)0x3C);
      if (j > 0 && j == ((counter01*18)+counter01+counter02)) {
        collectionData.add(j, (byte)0x3C);
        collectionData.add(j, (byte)0x3E);           
        counter01++;
        counter02++;
      }
    }   
    //add "{" in the begin of list
    for (int m = 0; m < collectionData.size(); m++) if (m == 0) collectionData.add(m, (byte)0x7B);    
    //add ">" in the end of list
    collectionData.add(collectionData.size(), (byte)0x3E);
    //add "}" in the end of list
    collectionData.add(collectionData.size(), (byte)0x7D);
    //store in ARRAY and test print   
    for (int k = 0; k < collectionData.size(); k++) {

      //println("["+k+"]"+"=" + collectionData.get(k)); 
      collectDataToArduino[k] = collectionData.get(k);
    }

    while (port.available () > 0) {

      int aChar = port.read();
      //println(aChar); 
      //0x33 "!" 
      if (aChar == 33) {
        port.write(collectDataToArduino);
        //0x3f "?"
        aChar=0x3f;
      }
    }
  }
}

//#define BOXNUM 18  //number of boxes 16
//#define NUMSLICES  9// number of "slices" = height //8 = max independent from serial buffer

//byte sinlabToArduino[] = new byte[];
/*
void sinlabLetter(){
  
  byte sinlab[] = {<111111000000000000><000000110000000000><000000000000110000><110000000000000000><000000111111000000>
                   <000000000000000011><000011000000000000><000000000011000000><000000000000111111>};
  
  port.write(sinlab);
  
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

