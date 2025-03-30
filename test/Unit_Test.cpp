#include <gtest/gtest.h>
#include "altimeter.h"

// We need to simulate Serial for testing. For simplicity, we can stub it out.
namespace {
  class SerialStub {
  public:
    void begin(unsigned long) {}
    bool operator!() { return false; }
    void println(const char*) {}
    void print(const char*) {}
    void print(float) {}
  } Serial;
}

// Declare the global variables from main.cpp as extern so we can reset them.
extern float previousAltitude;
extern bool parachuteDeployed;

// Declare the function we want to test.
bool processAltitude(float currentAltitude);

class AltimeterLogicTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Reset global variables before each test.
    previousAltitude = 100.0f;  // Assume an initial altitude of 100 m.
    parachuteDeployed = false;
  }
};

// Test that apogee is not detected if altitude does not drop significantly.
TEST_F(AltimeterLogicTest, NoApogeeDetection) {
  // Simulate a small altitude drop (< 0.5 m drop)
  bool deployed = processAltitude(99.8f);
  EXPECT_FALSE(deployed);
  EXPECT_FALSE(parachuteDeployed);
  EXPECT_FLOAT_EQ(previousAltitude, 99.8f);
}

// Test that apogee is detected when altitude drops by more than 0.5 m.
TEST_F(AltimeterLogicTest, DetectApogeeAndDeploy) {
  // Simulate a significant drop (e.g., from 100 m to 99.4 m: 0.6 m drop)
  bool deployed = processAltitude(99.4f);
  EXPECT_TRUE(deployed);
  EXPECT_TRUE(parachuteDeployed);
  EXPECT_FLOAT_EQ(previousAltitude, 99.4f);
}

// Test that once the parachute is deployed, further calls do not trigger deployment again.
TEST_F(AltimeterLogicTest, DoNotDeployMultipleTimes) {
  // First, simulate a drop that triggers deployment.
  bool deployed = processAltitude(99.4f);
  EXPECT_TRUE(deployed);
  // Simulate a secondary drop.
  deployed = processAltitude(98.0f);
  // The parachute should already be deployed.
  EXPECT_FALSE(deployed);
  EXPECT_TRUE(parachuteDeployed);
  EXPECT_FLOAT_EQ(previousAltitude, 98.0f);
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
  return RUN_ALL_TESTS();
}
#endif
