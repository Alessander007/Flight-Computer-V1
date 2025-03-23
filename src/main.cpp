#include <Arduino.h>
#include "altimeter.h"

float previousAltitude = 0.0;
bool parachuteDeployed = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }
  
  Serial.println("Serial Communication Started");

  // Initialize the BMP sensor.
  if (!initBMP()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);  // Halt execution if sensor initialization fails.
  }
  Serial.println("BMP sensor initialized successfully.");

  // Calibrate ground-level pressure (sets the baseline for altitude calculations)
  calibrateGroundPressure();
  
  // Capture an initial altitude reading
  previousAltitude = getAltitude();
}

void loop() {
  float currentAltitude = getAltitude();
  Serial.print("Approx. Altitude = ");
  Serial.print(currentAltitude);
  Serial.println(" m");

  // If the altitude decreases by more than 0.5 meter, assume apogee.
  if (!parachuteDeployed && (currentAltitude < previousAltitude - 0.5)) {
    Serial.println("Apogee detected. Deploying parachute...");
    // Insert your parachute deployment code here
    parachuteDeployed = true;
  }

  // Update previous altitude for next comparison
  previousAltitude = currentAltitude;
  
  delay(500);  
}
