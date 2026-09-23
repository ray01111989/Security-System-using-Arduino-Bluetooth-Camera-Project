# Security System using Arduino, Bluetooth and a phone camera

A door alarm built with an Arduino and a [1Sheeld](https://1sheeld.com/) (a Bluetooth shield that lets the Arduino use the features of a phone). When the door opens, the phone takes a photo of the intruder, sounds an alarm and speaks a warning. An RFID card reader checks a card and the phone announces the result.

This project was done in Arduino (class project, CSE 224). It is a base idea for a bigger project about the security of a house and tracking who gets in.

## How it works

1. An ultrasonic sensor measures the distance to the door every loop. If the distance is more than 10 cm, the door counts as **open**: the phone turns on its flash, takes a photo with the rear camera, the buzzer sounds for 2 seconds and the phone says "warning a stranger has entered your house".
2. The RFID reader waits for a card. If the card is the authorized one, the phone says "The door is open"; any other card gives "Access denied".

## What you need

- An Arduino board supported by 1Sheeld (the pin numbers below match an Arduino Uno)
- A 1Sheeld and a phone with the 1Sheeld app (camera, buzzer, terminal and text-to-speech shields enabled)
- An ultrasonic sensor: trigger on pin 6, echo on pin 7
- An MFRC522 RFID reader: SS on pin 10, RST on pin 9, plus the board's SPI pins
- The wiring diagram is in `schematic.png`

## Set up

1. In the Arduino IDE install the **1Sheeld** library and the **MFRC522** library (Library Manager).
2. Find the ID of your card: open `File > Examples > MFRC522 > DumpInfo`, upload it, hold the card to the reader and read the `Card UID` in the Serial Monitor (for example `5B FD 4D 0C`).
3. Copy `secrets.example.h` to `secrets.h` (same folder as the sketch) and write your card ID in it, for example `{0x5B, 0xFD, 0x4D, 0x0C}`. `secrets.h` is ignored by git, so your card ID is not published.
4. Open `final_code_for_the_project.ino`, choose your board and upload.
5. Pair the phone with the 1Sheeld in the 1Sheeld app and enable the shields listed above.

## Files

| File | What it is |
| --- | --- |
| `final_code_for_the_project.ino` | The Arduino sketch |
| `secrets.example.h` | Template for the file that holds the authorized card ID |
| `schematic.png` | Wiring diagram |
| `CSE 224 Project .pdf`, `Project alarm door 2019.pdf`, `projact for 224.png` | Documents and an image from the class project |

## Known limitations

- **A valid card does not switch the alarm off.** The alarm only looks at the distance, so if the door is opened after an authorized card was read, the photo, buzzer and warning still happen.
- **A card ID is weak security.** Anyone who can read the card can copy its ID. An earlier version of this repository published the ID of the card used in the project; treat that card as public and use a different one.
- If the ultrasonic sensor gets no echo, `pulseIn` waits up to a second and the distance reads 0, which the sketch treats as "door closed".
- The sketch was reviewed and its card check tested on a computer, but it was **not tested on the hardware**.
