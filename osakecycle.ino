#include <OneWire.h>
#include "Arduino.h"
#include <DallasTemperature.h>
#include "BluetoothSerial.h" 

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 25

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
BluetoothSerial ESP_BT;
float temp=0;

void setup(void)
{
  
  // start serial port
  pinMode (32, OUTPUT);
  pinMode (33, OUTPUT);
  pinMode (26, OUTPUT);
  pinMode (27, OUTPUT);
  Serial.begin(115200);
  ESP_BT.begin("ESP32ueno"); //Name of your Bluetooth Signal
  // Start up the library
  sensors.begin();
  Serial.println("なにか文字を送ってください");
  while(1)
  {
    Serial.print(".");
    if (Serial.available())
    {
      Serial.readStringUntil(';');
      break;
    }
    else if (ESP_BT.available())
    {
      ESP_BT.readStringUntil(';');
      break;
    }
    delay(1000);
  }
  Serial.println("準備完了");
}
unsigned long timer(void)
{
  unsigned long c=0;
  c=millis();
  return(c);
}
int cycle(float tem)
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  int key=0;
  Serial.print("Requesting temperatures...");
  ESP_BT.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  ESP_BT.println("DONE");
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));
  ESP_BT.println(sensors.getTempCByIndex(0));
  float check=abs(sensors.getTempCByIndex(0)-tem);
  if(check<=1.0)
  {
    digitalWrite(32, LOW);
    digitalWrite(33, LOW);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    key=1;
    ESP_BT.println("good");
  }
  else if(sensors.getTempCByIndex(0)<tem)
  {
    digitalWrite(32, HIGH);
    digitalWrite(33, LOW);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    ESP_BT.println("low");
  }
  else if(sensors.getTempCByIndex(0)>tem)
  {
    digitalWrite(32, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    ESP_BT.println("high");
  }
  return(key);
  delay(10);
}


void loop(void)
{ 
  unsigned long time=0;
  time=millis();
  while(1)
  {
    int key=cycle(95.0);
    if(key==1)
    {
     break; 
    }
  }
  time=millis();
  while(1)
  {
    int key=cycle(95.0);
    if(timer()-time>180000)
    {
     break; 
    }
  }
  int i=0;
  while(i<30)
  {
    while(1)
    {
      int key=cycle(95.0);
      if(key==1)
      {
       break; 
      }
    }
    time=millis();
    while(1)
    {
      int key=cycle(95.0);
      if(timer()-time>15000)
      {
       break; 
      }
    }
    while(1)
    {
      int key=cycle(59.0);
      if(key==1)
      {
       break; 
      }
    }
    time=millis();
    while(1)
    {
      int key=cycle(59.0);
      if(timer()-time>45000)
      {
       break; 
      }
    }
    i=i+1;
    Serial.println(i);
    ESP_BT.println(i);
  }
  while(1)
    {
      int key=cycle(15.0);
    }
}
