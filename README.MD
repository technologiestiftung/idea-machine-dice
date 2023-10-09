## Dice code for the idea machine

This C project can be used with platform.io & VSCode. Alternatively, you can install the dependencies manually and only copy the `main.cpp` file into the arduino IDE (rename to `main.ino`)

### What it does

It is supposed to run on a Heltec ESP32 Lora board. When successfully connected, the board will measure acceleration via the MPU6050 board and send a value to the defined server. The value would be the device's/cube's ID (A, B or C) and the side of the cube facing up (1,2,3,4,5 or 6), e.g. `E3`, `A2`.

Rename the `condig-example.h` file to `config.h` and adjust the values to your setup:

````

````

### Bluetooth Setup:

1. You need to pair the board with the raspberry pi.
2. Check if `bluetoothctl` is installed on the pi. If not, install it.
3. Install `picocom` on the pi:
```shell
sudo apt-get install picocom
```
4. bind the esp32 to a serial port on the raspberry pi (needs to be done every time the pi is restarted):
```shell
sudo rfcomm bind rfcomm0 <MAC_ADDRESS_OF_ESP32_BOARD>
```
5. You can then run the following command on the raspberry pi to receive the data from the board:
```shell
picocom /dev/rfcomm0 -b 115200
```