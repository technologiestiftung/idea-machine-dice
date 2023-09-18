## Dice code for the idea machine

This C project can be used with platform.io & VSCode. Alternatively, you can install the dependencies manually and only copy the `main.cpp` file into the arduino IDE (rename to `main.ino`)

### What it does

It is supposed to run on a Heltec ESP32 Lora board. When successfully connected, the board will measure acceleration via the MPU6050 board and send a value to the defined server. The value would be the device's/cube's ID (A, B or C) and the side of the cube facing up (1,2,3,4,5 or 6), e.g. `E3`, `A2`.

Rename the `condig-example.h` file to `config.h` and adjust the values to your setup:

````
const char* ssid = "YOUR SSID";
const char* password = "YOUR WIFI PASSWORD";
const char* server = "YOUR URL"; // Server URL
const int port = 3000;           // insert your port
const char* token = "YOUR BEARER TOKEN";
const char* endpoint = "PATH/TO/ENDPOINT";
const char myID[] = "A"; // One of A, B, C
````
