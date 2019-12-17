/*
  Security System using Arduino Bluetooth Camera Project

  This project shows an application on 1Sheeld's camera shield.

  By using this project, you can track whether anything stranger
  has entered your house and capture his photo  by using camera
  shield from 1Sheeld.
  
*/

/* Include required shields */
#define CUSTOM_SETTINGS
#define INCLUDE_CAMERA_SHIELD
#define INCLUDE_BUZZER_SHIELD
#define INCLUDE_TERMINAL_SHIELD
#define INCLUDE_TEXT_TO_SPEECH_SHIELD

/* A command which will be converted to speech */
const char command[] = "warning a stranger has entered your house";
const char com[] = "The door is open";
const char co[] = "Access denied";
/* Include 1Sheeld library. */
#include <OneSheeld.h>
#include <MFRC522.h>
#include <SPI.h>
/* Define pins for the ultrasonic sensor */
const int trigPin = 6;
const int echoPin = 7;
const String targetUID = "5B FD 4D 0C";
/* Variable to save the measured distance from ultrasonic */
int distance;
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
void loop()
{
    distance = getUltrasonicDistance();
    Terminal.println(distance);
   // String content= "";
    if ( distance > 10)
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
    TextToSpeech.say(command);

    /* Delay for 5 seconds to give the camera shield enough time for taking and saving the photo */
    OneSheeld.delay(5000);
  }
  //here we have to wait for the card, when it is near to the sensor
if ( ! mfrc522.PICC_IsNewCardPresent()){
return;
}
//we can read it's value
if ( ! mfrc522.PICC_ReadCardSerial()) {
return;
}

Serial.print("Tag:");
String content= "";
for (byte i = 0; i < mfrc522.uid.size; i++) {
Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
Serial.print(mfrc522.uid.uidByte[i], HEX);
content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
content.concat(String(mfrc522.uid.uidByte[i], HEX));
}
content.toUpperCase();
content = content.substring(1);

  if(content == "5B FD 4D 0C" && distance < 10){
   TextToSpeech.say(com);
   delay(10000);
  }
  else{

   content == "56 60 28 F8";
    TextToSpeech.say(co);
  }

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
