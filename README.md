# This is a slightly modified version of the BearSSL sample server for the ESP8266 Arduino framework using PlatformIO

## Compile

first copy and edit the `lib/private/PrivateConfig.hpp.sample` file to `lib/private/PrivateConfig.hpp`

## Notice

The code crashes if you run it on 80MHz CPU frequency, you have to switch to 160MHz to stabilize the program.

`board_build.f_cpu = 160000000L` in `platformio.ini`

## Screenshot

![Screenshot](./img/screenshot.png)

## Simple CAv3 (Certificate Authority)

find it [here](https://github.com/thorsten-l/Simple-CAv3).

## References

- [HelloServerBearSSL](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/examples/HelloServerBearSSL/HelloServerBearSSL.ino)
- [WEMOS/LOLIN D1 mini Pro](https://wiki.wemos.cc/products:d1:d1_mini_pro)
