
#include <Arduino.h>

const int pingPin = 12; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 11; // Echo Pin of Ultrasonic Sensor
long duration;          // duration of signal

void setup()
{
  // setup pin directions
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200); // Starting Serial Terminal
}

void loop()
{
  duration = readUltraSonic();
  Serial.print("Total flight time: ");
  Serial.print(duration);
  Serial.print("us, total distance: ");
  Serial.print(microseconds2Millimeters(duration));
  Serial.print("mm");
  Serial.println();
  delay(250); // rest for a while
}
long readUltraSonic()
{
  /*
Reads the ultrasonic sensor
This is a standart function where
a ping signal is sent through trigger pin
and an echo is expected in return
*/
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  // listen for echo and return it, and use a time out since
  // sensor has a range of 4 meters, why is there a 25000
  // pleare read the help for pulseIn
  return pulseIn(echoPin, HIGH, 25000);
}
long microseconds2Millimeters(long microseconds)
{
  /*
convert microsecond to centimeters:
speed of sound is 343 m / s * 1000 mm / 1 m
speed of sound is 343 * 1000 mm / s * 1 s / 1000000 us
speed of sound is 0.343 mm / us
however, use of floating point is not a good idea here
best long integer approximation for us to mm is:
note that sound travels back and forth
hence distance to the obstacle shoould be half the distance
that the sound wave traveled... note this fact.
to get a proper result update the following return expression
so that returned value is the best integer approxiamtion of
the sensed distance
NOTE that you have to fix this so that the mm values returned
matches the cm values displayed when you click on the sensor
and move the target circle around
*/
  return microseconds * 1 / 1;
}