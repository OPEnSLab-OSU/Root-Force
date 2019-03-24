/***************************************************************
  Device #:
  Calibration:

  Code version V1
  
  Description:
      Code for RootForce sensors to log magnitudes of Force
      exerted by roots on environment. Includes code for calibration,
      SD card logging, and adjustabe software filtering.

  Setup:
      Calculations are made based on calibrations from known weight forces.
      Applying a 200kg weight to the load cell and observing the change in
      voltage should result in a linear graph where voltage increases as
      more weight is applied in the direction of gravity.

      This code shoulb be used after calibrating based on known weight forces
      (kg * m/s^2) and can give an approximation of Force magnitude in Newtons.

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

/*********** Change these values ******************************/
// sensor name
const int SensorNum = 1;      // assign this sensor a number

// sensor type (tekscan or load cell)
const String type = "Tekscan";  // Does not appear in fileaname

// exponential filter values
const float a = 0.7;         // increase to add weight to new values

/*********** End of change values *****************************/

String fileName = "RF_" + String(SensorNum) + ".txt";

// MUST initialize "values" to 0
float values[2] = {0, 0}; // size container for previous value and current
float output = 0;         // result of applying exponential filtering (requires time to settle dependant on chosen alpha)

//const long cal = ;
const long g = 9.81;

void setup() {
  Serial.begin(9600);

  if (!SD.begin(ChipSelSD)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }

  // writes calibration info to card upon init
  if (!Write_info()) {
    Serial.println("Card failed to store init data ");
    // don't do anything more:
    while (1);
  }
}

void loop() {
  int val = analogRead(A0);
  float volt = (3.3 / 1024) * val;
  Serial.println(volt);
  delay(100);

  LPF_filter(volt);    // raw value to add to time series
  Serial.println(output); // should show the 
  
  // need to add calibration info
  
  // uncomment to write to SD
  /*
    File dataFile = SD.open("RootForce1", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
    dataFile.println(output);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
    Serial.println("error opening datalog.txt");
    }
  */
}

void LPF_filter(float raw_input) {
  values[1] =  raw_input;
  output = (a * values[0] + values[1]) / (a + 1);
  values[0] = values[1];  //copy to new raw input to be the next prev input
  return;
}

bool Write_info() {
  File data = SD.open(fileName, FILE_WRITE);
  // if the file is available, write to it:
  if (data) {
    data.println(type);
    data.print("Sensor Number: ,");
    data.println(SensorNum);
    data.print("Calibration factor a: ,");
    data.println(a);
    data.println("********** Begin recording data ***********");
    data.close();
    // print to the serial port too:
    Serial.println(type);
    Serial.print("Sensor Number: ");
    Serial.println(SensorNum);
    Serial.print("Calibration factor a: ");
    Serial.println(a);
    return 1;
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
    return 0;
  }
}
