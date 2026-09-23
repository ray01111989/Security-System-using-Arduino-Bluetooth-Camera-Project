/*
  Security System using Arduino Bluetooth Camera Project

  This project shows an application on 1Sheeld's camera shield.

  By using this project, you can track whether anything stranger
  has entered your house and capture his photo  by using camera
  shield from 1Sheeld.

  How it works
  - An ultrasonic sensor watches the door. When the measured distance is more than DOOR_OPEN_DISTANCE_CM
    the door is treated as open: the phone takes a photo, the buzzer sounds and the phone speaks a warning.
  - An RFID reader checks cards. The authorized card makes the phone say "The door is open";
    any other card makes it say "Access denied".

  Before you upload
  - Copy secrets.example.h to secrets.h and put the ID of your own card in it (see the README).
    secrets.h is not committed, so the card ID is not published.
*/

/* Include required shields */
#define CUSTOM_SETTINGS
#define INCLUDE_CAMERA_SHIELD
#define INCLUDE_BUZZER_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD

/* Include 1Sheeld library. */
#include <OneSheeld.h>
#include <MFRC522.h>
#include <SPI.h>
#include "secrets.h"   /* defines AUTHORIZED_UID, the ID of the card that may open the door */

/* Messages that are converted to speech */
const char alarmMessage[] = "warning a stranger has entered your house";
const char doorOpenMessage[] = "The door is open";
const char deniedMessage[] = "Access denied";

/* Define pins for the ultrasonic sensor */
const int trigPin = 6;
const int echoPin = 7;

/* The door counts as open when the sensor measures more than this many centimeters */
const int DOOR_OPEN_DISTANCE_CM = 10;

/* Variable to save the measured distance from ultrasonic */
int distance;

/* Pins for the RFID reader (the SPI pins are fixed by the board) */
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  pinMode(trigPin, OUTPUT);     // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);      // Sets the echoPin as an Input
  /* Start 1Sheeld communication. */
  OneSheeld.begin();
  SPI.begin();
  mfrc522.PCD_Init();
}

/* Returns true only if the card ID (uid, of length size bytes) is exactly the authorized one.
   Comparing raw bytes replaces the old String comparison, which used a lot of memory on the Arduino. */
bool isAuthorizedUid(const byte *uid, byte size)
{
  if (size != sizeof(AUTHORIZED_UID))
  {
    return false;               // a card with a different ID length can never match
  }
  for (byte i = 0; i < size; i++)
  {
    if (uid[i] != AUTHORIZED_UID[i])
    {
      return false;
    }
  }
  return true;
}

void loop()
{
  distance = getUltrasonicDistance();
  Terminal.println(distance);
  if (distance > DOOR_OPEN_DISTANCE_CM)
  {
    /* Turn on the camera's flash while capturing */
    Camera.setFlash(ON);

    /* Take a photo for that stranger using the phone's rear camera */
    Camera.rearCapture();

    /* Also turn the alarm in form of a buzzer */
    Buzzer.buzzOn();

    /* Delay for 2 seconds */
    OneSheeld.delay(2000);

    /* Turn the alarm off */
    Buzzer.buzzOff();

    /* Use text-to-speech shield to announce a break into security orally */
    TextToSpeech.say(alarmMessage);

    /* Delay for 5 seconds to give the camera shield enough time for taking and saving the photo */
    OneSheeld.delay(5000);
  }

  /* Here we have to wait for the card, when it is near to the sensor */
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  /* We can read its ID */
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  if (isAuthorizedUid(mfrc522.uid.uidByte, mfrc522.uid.size))
  {
    TextToSpeech.say(doorOpenMessage);
    delay(10000);
  }
  else
  {
    TextToSpeech.say(deniedMessage);
  }

  /* Tell the card to stop and end the secure session, so the reader can detect the next card */
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

/* A function that makes the whole operation of the ultrasonic and returning the detected distance */
long getUltrasonicDistance(void)
{
  /* Variable to save the sound wave travel time in microseconds */
  long duration;

  /* Variable to save the detected distance in cm */
  long distanceReturned;

  /* Clears the trigPin */
  digitalWrite(trigPin, LOW);

  /* delay 2 micro seconds */
  delayMicroseconds(2);

  /* Sets the trigPin on HIGH state for 10 micro seconds */
  digitalWrite(trigPin, HIGH);

  /* delay 10 micro seconds */
  delayMicroseconds(10);

  /* Sets the trigPin on LOW state */
  digitalWrite(trigPin, LOW);

  /* Reads the echoPin, returns the sound wave travel time in microseconds */
  duration = pulseIn(echoPin, HIGH);


  /* Calculating the distance */
  distanceReturned = duration * 0.034 / 2;

  /* Returning the detected distance in cm */
  return distanceReturned;
}
