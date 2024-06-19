// Basic demo for accelerometer readings from Adafruit MPU6050
#include "config.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "BluetoothSerial.h"

Adafruit_MPU6050 mpu;

const int debug = 0;
int last_side = 0;
int movement_counter = 0;

BluetoothSerial SerialBT;

void measure()
{
  // Get new sensor events with the readings 
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if (debug == 1)
  {
    // Print out the values 
    Serial.print("X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println("");
  }

  
  //   Sides (like on a real 1-6 die)
  //   1 = z+
  //   2 = x-
  //   3 = y-
  //   4 = y+
  //   5 = x+
  //   6 = z-
  

  int side = 0;
  if (a.acceleration.x >= +8)
    side = 5;
  if (a.acceleration.x <= -8)
    side = 2;
  if (a.acceleration.y >= +8)
    side = 4;
  if (a.acceleration.y <= -8)
    side = 3;
  if (a.acceleration.z >= +8)
    side = 1;
  if (a.acceleration.z <= -8)
    side = 6;

  if (side != last_side && side != 0)
  {
    last_side = side;
    movement_counter = 0;
    Serial.print("Seite: ");
    Serial.println(side);
  }

  if (side == last_side && side != 0)
  {
    movement_counter++;
  }

  if (movement_counter == 2)
  {
    movement_counter++;
    Serial.println("....sending data...");
    Serial.print(side);
    Serial.println("");

    SerialBT.println(side);
  }
}

void setup(void)
{
  Serial.begin(115200);

  Serial.println("--------------------");

  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
  }
  else
  {
    Serial.println("MPU6050 Found!");
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  SerialBT.begin(name); // Name of the ESP32
  Serial.println("The ESP32 is ready. You can now connect via Bluetooth.");
}

void loop()
{
  measure();
  delay(500);
}
