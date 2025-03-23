#include <Arduino.h>
#include <Adafruit_BMP3XX.h>

// Sensor instance for the BMP3XX sensor
extern Adafruit_BMP3XX bmpSensor;

// Reference ground-level pressure (in hPa) used for altitude calculations.
extern float groundPressure_hPa;

// Initializes the BMP sensor. Returns true on success.
bool initBMP();

// Returns the current altitude (in meters) calculated using the sensor reading.
float getAltitude();

// Calibrates the ground-level pressure before launch.
void calibrateGroundPressure();