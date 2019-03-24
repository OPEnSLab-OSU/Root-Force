/***************************************************************
  Device #:
  Calibration:

  Code version
  Description:
      Code for RootForce sensors to log magnitudes of Force
      exerted by roots on environment. Includes code for calibration,
      SD card logging, and low power mode.

  Setup:
      Calculations are made based on calibrations from known weight forces.
      Applying a 200kg weight to the load cell and observing the change in
      voltage should result in a linear graph where voltage increases as
      more weight is applied in the direction of gravity.

      This was used to calibrate for forces based on known weight forces
      (kg * m/s^2) and can give an approximation in Newtons when the load
      cell is turned 90 degrees on its side.

  Wiring:
      3V  - Red Wire of Load Cell
      GND - Black Wire of Load Cell
      A0  - Blue Wire out of INA125P Amp

  Marissa Kwon 3/7/19
***************************************************************/
// Libraries
#include <SPI.h>
#include <SD.h>

#define LoadCell1 A0
#define ChipSelSD 4

long output = 0;
float calib_factor = 0; // need to set after calibrating
//const long cal = ;
const long g = 9.81;

void setup() {
  Serial.begin(9600);

  if (!SD.begin(ChipSelSD)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  
}

void loop() {
  int val = analogRead(A0);
  float volt = (3.3/1024)*val;
  Serial.println(volt);
  delay(100);
 
  //output = calib_factor*volt*g; // output force
  
  // uncomment if writing to SD to get linear trend, std deviation etc.;
  /*
  File dataFile = SD.open("RootForce1", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(volt);  // replace with output to log expected force measurements
    dataFile.close();
    // print to the serial port too:
    Serial.println(volt);    // replace with output to log expected force measurements
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  */
}

