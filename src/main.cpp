// Basic demo for accelerometer readings from Adafruit MPU6050
#include "config.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFiClientSecure.h>

Adafruit_MPU6050 mpu;

const int debug = 0;
int last_side = 0;
int movement_counter = 0;

const int BUTTON_PIN = 0;
bool buttonState = false;
bool lastButtonState = false;

#define LED_PIN 25

WiFiClientSecure client;

void sendValueToServer(const String &value)
{

  Serial.println("\nConnecting to " + String(server));
  client.setInsecure(); // skip verification

  if (!client.connect(server, port))
    Serial.println("Connection failed.");
  else
  {
    Serial.println("Connected.");

    client.println("POST " + String(endpoint) + " HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Authorization: Bearer " + String(token));
    client.print("Content-Length: ");
    client.println(String(strlen("shortcode=XX")));
    client.println();
    client.print("shortcode=");
    client.println(value);

    while (client.connected())
    {
      String line = client.readStringUntil('\n');
      if (line == "\r")
      {
        Serial.println("headers received");
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available())
    {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  }
}

void blinkLED(int numBlinks)
{
  pinMode(LED_PIN, OUTPUT);

  for (int i = 0; i < numBlinks; i++)
  {
    digitalWrite(LED_PIN, HIGH); // LED einschalten
    delay(500);                  // 500 Millisekunden warten
    digitalWrite(LED_PIN, LOW);  // LED ausschalten
    delay(500);                  // 500 Millisekunden warten
  }
}

void startWifi()
{
  // Verbindung zum Wi-Fi-Netzwerk herstellen
  WiFi.begin(ssid, password);
  // WiFi.begin(ssid);

  Serial.print("Connecting to ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected.");
}

void stopWifi()
{
  // Wi-Fi-Verbindung trennen
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("Wi-Fi-Verbindung getrennt.");
}

void measure()
{
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if (debug == 1)
  {
    /* Print out the values */
    Serial.print("X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println("");
  }

  /*
   *  Sides (like on a real 1-6 die)
   *  1 = z+
   *  2 = x-
   *  3 = y-
   *  4 = y+
   *  5 = x+
   *  6 = z-
   *
   */

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
    char roll[10];
    sprintf(roll, "%c%d", myID[0], side);
    Serial.println("....sending data...");
    Serial.print(roll);
    Serial.println("");

    sendValueToServer(roll);
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

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  startWifi();
  Serial.print("ID: ");
  Serial.println(myID);
}

void loop()
{
  measure();
  delay(500);
}
