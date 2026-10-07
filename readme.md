# Mini Arduino Bomb

*Made by DaviXG7, Alex AR and Davi Alves.*

We decided to share it with the community for educational purposes and we made it at school and it was a fun project to learn about electronics and programming.

# Circuit diagram
*It isn't accurate because of the Tinkercad application doesn't have all the components we used*

<img width="1880" height="854" alt="Exquisite Gogo" src="https://github.com/user-attachments/assets/0a81c6e6-dd77-4ca4-a13f-f51f23766374" />

Diagram PDF from Tinkercad: [Exquisite Gogo.pdf](https://github.com/user-attachments/files/33131208/Exquisite.Gogo.pdf)

## About project

This project is a simulation of a bomb in a _game prop_ / _timer_ style inspired by defuse games, developed with Arduino Uno and PlatformIO.

The system uses a 7-segment display, a matrix keypad, and a buzzer to create a countdown experience, secret code input, and user interaction.

## Overview

The bomb starts in standby mode and is only activated when pressing key `1`.
Once armed, the timer begins a 30-second countdown. The player must enter the correct secret code to disarm the bomb before time runs out.

If the code is wrong, the timer is reduced by 5 seconds. If the countdown reaches zero, the bomb explodes.

## Features

- 30-second countdown timer
- Matrix keypad for command input
- 4-digit display with multiplexing
- Buzzer feedback for actions and alarms
- Secret code system to disarm the device
- Quick reset with key `9`
- Ready-to-use PlatformIO project structure

## Hardware used

- Arduino Uno
- 4-digit 7-segment display (common cathode)
- 4x4 matrix keypad configured as 3x3
- Passive buzzer
- Wires, resistors, and protoboard

## Pin mapping

### 7-segment display
- Segments A-G: digital pins `2` to `8`
- Digits D1-D4: digital pins `9` to `12`

### Matrix keypad
- Rows: `A0`, `A1`, `A2`
- Columns: `A3`, `A4`, `A5`

### Buzzer
- Pin: `13`

## Project controls

- `1`: arms the bomb and starts the countdown
- `7`, `5`, `3`: secret code to disarm the bomb
- `9`: resets the game after explosion or successful disarm

## Secret code

The configured code in the project is:

- `7` → `5` → `3`

## Game states

- `WAITING`: bomb inactive
- `ARMED`: countdown active
- `DISARMED`: bomb successfully disarmed
- `EXPLODED`: bomb exploded

## Project structure

```text
Bomba/
├── include/          # Header files
├── lib/              # Local project libraries
├── src/
│   └── main.cpp      # Main source code
├── platformio.ini    # PlatformIO configuration
├── ReadMe.md         # Project documentation
└── README.md         # Optional if you want an English version
```

## Dependencies

This project uses the following library:

- `Keypad` by `chris--a`

The dependency is already configured in `platformio.ini`.

## Build instructions

With PlatformIO installed, run:

```bash
pio run
```

## Upload to Arduino

```bash
pio run -t upload
```

## Open serial monitor

```bash
pio device monitor
```

## How to use

1. Connect the components according to the pin mapping.
2. Upload the code to the Arduino.
3. Press `1` to activate the bomb.
4. Enter the secret code to disarm it.
5. If the code is wrong, 5 seconds are subtracted from the timer.
6. Press `9` to reset the game.

## Notes

- The code was designed for an Arduino Uno with a 7-segment display and analog keypad.
- The `Keypad` library simplifies reading the matrix keypad input.
- The sound and visual effects were designed to simulate a bomb environment inspired by _C4 / CS:GO_.

## Possible improvements

- Add serial monitor messages
- Create a difficulty selection menu
- Add status indicator LEDs
- Make the initial timer and password configurable
- Add additional game modes

## License

This project was created for educational and learning purposes in electronics and Arduino development.

---

# Videos

https://github.com/user-attachments/assets/b7dc0989-e471-49a0-8a3f-e662f960b059

https://github.com/user-attachments/assets/63e9676f-adcb-4427-8a42-5966042efeae
