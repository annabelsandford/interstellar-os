# Interstellar OS

Interstellar OS is a serial-input text-based Operating System for the Arduino Nano / Uno running @ 9600 Baud.

## Installation

1) Download & Install the [Arduino IDE](https://www.arduino.cc/en/software)
2) Then download [os.ino]()
3) Open / import os.ino into the Arduino IDE
4) Connect your Arduino via USB
5) Upload os.ino onto your Arduino

## Modifying
You can modify OS variables at the beginning of [os.ino]()

_Change the value (normally set to 130) to change the EEPROM Address where Interstellar writes the usrnm_set (Is a username set or not (0/1)) int_
```c
uint8_t eead = 130;
```
_Change the username character limit (Normally set to 4 to not interfere with other bytes saved within the EEPROM)_
```c
int usrnm_limit = 4;
```
_Change the string value to whatever character you want Interstellar to use as a line break_
```c
String br = "=";
```
_Those are system checks strings. Editing those is not recommended as system functions would not be able to be called._
```c
String del = "userdelete";
String inf = "info";
String hi = "hi";
String help = "help";
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
