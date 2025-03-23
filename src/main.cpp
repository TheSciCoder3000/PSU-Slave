#include "Wire.h"
#include "Arduino.h"
#include "math.h"
#include "XY6015_CID.h"

#define button 26
#define SLAVE_ADD 69

bool i2cAvailable = false;
void receiveEvent(int);
void requestEvent();

bool isPressed = false;
bool isOn = false;

XY6015 psu4;
XY6015 psu5;
XY6015 psu6;

void setVoltage(int index, float voltage);
void setCurrent(int index, float current);
void read(int index);
void toggle(int index, bool state);

String received = "";
String requestOutput = "";

void setup()
{
  Wire.begin(SLAVE_ADD);
  Wire.onReceive(receiveEvent); // register event
  // Wire.onRequest(requestEvent);
  Serial.begin(9600);

  psu4.begin(115200, &Serial2);
  psu5.begin(115200, &Serial3);
  psu6.begin(115200, &Serial1);
}

void loop()
{
  delay(100);
  // Serial.println("Loop");
  if (i2cAvailable)
  {
    i2cAvailable = false;

    String outputType = received.substring(1, 5); // select the first 4 characters of command
    Serial.println(received);
    int index = received.substring(0, 1).toInt();

    if (outputType == "SETV")
    {
      float voltage = received.substring(5, received.length()).toFloat();
      setVoltage(index, voltage);
      Serial.println("SETV: " + String(voltage));
    }
    else if (outputType == "SETA")
    {
      float curr = received.substring(5, received.length()).toFloat();
      setCurrent(index, curr);
      Serial.println("SETA: " + String(curr));
    }
    else if (outputType == "READ")
    {
      read(index);
      Serial.println("READSLAVE ");
    }
    else if (outputType == "TOGG")
    {

      int state = received.substring(5, 6).toInt();
      toggle(index, state);

      Serial.println("TOGG: " + String(index));
    }
    else
    {
      Serial.println("Invalid: " + received);
    }
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }      // print the integer
}

void requestEvent()
{
  psu4.awaitResponse();
  psu5.awaitResponse();
  psu6.awaitResponse();
}

void setVoltage(int index, float voltage)
{
  if (index == 4)
    psu4.setVoltage(voltage);
  if (index == 5)
    psu5.setVoltage(voltage);
  if (index == 6)
    psu6.setVoltage(voltage);
}
void setCurrent(int index, float current)
{
  if (index == 4)
    psu4.setCurrent(current);
  if (index == 5)
    psu5.setCurrent(current);
  if (index == 6)
    psu6.setCurrent(current);
}
void read(int index)
{
  if (index == 4)
    psu4.read();
  if (index == 5)
    psu5.read();
  if (index == 6)
    psu6.read();
}
void toggle(int index, bool state)
{
  if (index == 4)
    psu4.toggle(state);
  if (index == 5)
    psu5.toggle(state);
  if (index == 6)
    psu6.toggle(state);
}
