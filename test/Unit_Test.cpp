// fake_bmp3xx.h
#pragma once
#include <Adafruit_BMP3XX.h>

class FakeBMP3XX : public Adafruit_BMP3XX {
public:
  bool fakePerformReading;
  float fakePressure; // in Pa
  float fakeAltitude;

  FakeBMP3XX()
    : fakePerformReading(false), fakePressure(101325.0f), fakeAltitude(0.0f) {}

  // Override performReading to return our fake value.
  bool performReading() override {
    return fakePerformReading;
  }

  // Override readAltitude to return our fake altitude.
  float readAltitude(float groundPressure) override {
    return fakeAltitude;
  }

  // Override pressure member access if needed.
  // (You may need to expose pressure in your fake sensor depending on how your code accesses it.)
};


// test_altimeter.cpp
#include <gtest/gtest.h>
#include "altimeter.h"
#include "fake_bmp3xx.h"

// Use a fake sensor instance for testing.
FakeBMP3XX fakeSensor;

// Before tests run, point bmpSensor to the fake sensor.
class AltimeterTestFixture : public ::testing::Test {
protected:
  void SetUp() override {
    bmpSensor = &fakeSensor;
    // Reset ground pressure to default before each test.
    groundPressure_hPa = 1013.25f;
  }
};

// Test that getAltitude returns 0.0f if performReading() fails.
TEST_F(AltimeterTestFixture, GetAltitudeFailsReturnsZero) {
  fakeSensor.fakePerformReading = false;
  float altitude = getAltitude();
  EXPECT_FLOAT_EQ(altitude, 0.0f);
}

// Test that calibrateGroundPressure sets the groundPressure_hPa correctly.
TEST_F(AltimeterTestFixture, CalibrateGroundPressureUpdatesPressure) {
  fakeSensor.fakePerformReading = true;
  fakeSensor.fakePressure = 101500.0f; // Pressure in Pa; expected ground pressure is 1015.0 hPa.
  // To simulate the behavior in calibrateGroundPressure, you may need to modify your code 
  // so that it reads fakeSensor.fakePressure. One approach is to expose a getter in FakeBMP3XX.
  // For this example, we assume bmpSensor->pressure returns fakeSensor.fakePressure.
  calibrateGroundPressure();
  EXPECT_NEAR(groundPressure_hPa, 1015.0f, 0.1f);
}

// Test that getAltitude returns the simulated altitude when performReading() is successful.
TEST_F(AltimeterTestFixture, GetAltitudeReturnsSimulatedValue) {
  fakeSensor.fakePerformReading = true;
  fakeSensor.fakeAltitude = 250.0f;
  float altitude = getAltitude();
  EXPECT_NEAR(altitude, 250.0f, 0.1f);
}

#if defined(ARDUINO)
#include <Arduino.h>
void setup() {
    Serial.begin(115200);
    ::testing::InitGoogleTest();
}
void loop() {
    RUN_ALL_TESTS();
    delay(1000);
}
#else
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}
#endif

