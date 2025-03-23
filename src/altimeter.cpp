#include "altimeter.h"

// Define the sensor instance and ground pressure variable.
Adafruit_BMP3XX bmpSensor;
float groundPressure_hPa = 1013.25;  // default sea level pressure

// Initialize the BMP3XX sensor using I2C
bool initBMP() {
  // Attempt to initialize the sensor via I2C
  if (!bmpSensor.begin_I2C()) {  
    return false;
  }
  // Configure sensor settings (as per datasheet recommendations)
  bmpSensor.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmpSensor.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmpSensor.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmpSensor.setOutputDataRate(BMP3_ODR_50_HZ);
  
  return true;
}

// Returns the current altitude (in meters)
float getAltitude() {
  // Perform a reading from the sensor
  if (!bmpSensor.performReading()) {
    return 0.0f;  // Return 0 on failure (you could handle this differently)
  }
  // Calculate altitude using the ground pressure as the baseline.
  return bmpSensor.readAltitude(groundPressure_hPa);
}

// Calibrate the ground-level pressure; call this before launch.
void calibrateGroundPressure() {
  if (bmpSensor.performReading()) {
    // Convert pressure from Pa to hPa
    groundPressure_hPa = bmpSensor.pressure / 100.0f;
  }
}
