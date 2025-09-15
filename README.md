# espjoy
A simple ESP32 BLE HID gamepad

Demo video here: https://www.youtube.com/watch?v=CmIby3AIlVc

This is a great example project for implementing a working bluetooth joystick controller using ESP32.

It sets up all of the required bluetooth attributes needed to be recognized as a joystick device on modern computers.

You can adapt the code to custom hardware as needed. The codebase is quite straightforward and easy to modify.

It can be a bit tricky to get bluetooth working correctly, especially if you don't use the correct set of attributes.

The attributes used by this project configure the joystick like an xbox controller, which is nearly universally supported. 

The device created by the code will also be detected and work in JavaScript browser environments as well.
