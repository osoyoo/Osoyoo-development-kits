// SdRaw_Serial_Uploader
// Copyright (C)2015 Graham Lawrence (GHLawrence2000). All Rights reserved.
// web: https://github.com/ghlawrence2000/UTFT_SdRaw
//
// This program is the Processing companion sketch for use with the Arduino 
// SdRaw_Serial_Demos
//

import processing.serial.*;
int     Serial_SPEED=115200; // Speed is irrelevent if using SerialUSB on DUE!!
Serial  port = null;
int     capacity;
int     mysize;
int     mytime;
int     tt;
int     min;
int     sec;
boolean done = false;
// Wait for line from serial port, with timeout
String readLine() {
  String s;
  int    start = millis();
  do {
    s = port.readStringUntil('\n');
  } while ( (s == null) && ((millis() - start) < 3000));
  return s;
}
// Extract unsigned multibyte value from byte array
int uvalue(byte b[], int offset, int len) {
  int    i, x, result = 0;
  byte[] bytes = java.util.Arrays.copyOfRange(b, offset, offset + len);
  for (i=0; i<len; i++) {
    x = bytes[i];
    if (x < 0) x += 256;
    result += x << (i * 8);
  }
  return result;
}
void setup() {
  String s;
  size(200, 200); // Dummy window for Serial
  println("Scanning serial ports...");
  for (String portname : Serial.list ()) {
    try {
      port = new Serial(this, portname, Serial_SPEED);
    } 
    catch (Exception e) { // Port in use, etc.
      continue;
    }
    print("Trying port " + portname + "...");
    delay(1500); // uncomment this line if using AVR
    port.write("HELLO\n");
    delay(20);
    if (((s = readLine()) != null) && s.contains("OK")) {
      println("OK!");
      break;
    } else {
      println();
      port.stop();
      port = null;
    }
  }
  if (port != null) { // Find one?
    selectInput("Select a file to process:", "fileSelected");
  }
}

void fileSelected(File f) {
  if (f == null) {
    println("Cancel selected or window closed.");
  } else {
    println("Selected file: " + f.getAbsolutePath());
    byte input[] = loadBytes(f.getAbsolutePath());
    port.write("IMAGE");
    mytime=millis();
    port.write(input);
    println("Time taken "+(millis()-mytime)+" ms");
    //delay(100000);
  } 
  done = true;
}

void draw() {
  if (done) exit();
}