#include <OneWire.h>
#include "Arduino.h"
#include <DallasTemperature.h>
#include "BluetoothSerial.h"

//25番につながっている何かのセンサ？？温度？？
#define ONE_WIRE_BUS 25
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

BluetoothSerial ESP_BT;
float temp = 0;

void setup(void)
{
  // start serial port
  pinMode (32, OUTPUT);
  pinMode (33, OUTPUT);
  pinMode (26, OUTPUT);
  pinMode (27, OUTPUT);
  Serial.begin(115200);
  ESP_BT.begin("ESP32ueno");

  sensors.begin();
  Serial.println("なにか文字を送ってください");
  //最後はセミコロン終わり？

  while (1) {
    //ここの処理、PC側と無線側どちらかが完了していなくても進むけど大丈夫ですか？
    Serial.print(".");
    if (Serial.available()) {
      Serial.readStringUntil(';');
      break;
    }
    //ここにelse if使う意味がない気がしたので削除しました
    if (ESP_BT.available()) {
      ESP_BT.readStringUntil(';');
      break;
    }
    delay(100);
  }
  Serial.println("準備完了");
}

unsigned long timer(void) {
  //  unsigned long c = 0;
  //  c = millis();//ミリ秒
  //  return (c);
  return (millis()); //<-これしてるだけじゃね？(この関数の存在意義なさそう)
}

int cycle(float tem) {
  //ここはセンサーのライブラリコピペですかね...？？
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  int key = 0;
  Serial.print("Requesting temperatures...");
  ESP_BT.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  ESP_BT.println("DONE");
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));
  ESP_BT.println(sensors.getTempCByIndex(0));
  float check = abs(sensors.getTempCByIndex(0) - tem);
  if (check <= 1.0) {
    digitalWrite(32, LOW);
    digitalWrite(33, LOW);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    key = 1;
    ESP_BT.println("good");
  } else if (sensors.getTempCByIndex(0) < tem) {
    digitalWrite(32, HIGH);
    digitalWrite(33, LOW);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    ESP_BT.println("low");
  } else if (sensors.getTempCByIndex(0) > tem) {
    digitalWrite(32, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    ESP_BT.println("high");
  }

  return (key);
  delay(10); // <-これ要ら無くないですか？
}

unsigned long mytime;
void loop(void) {
  //  unsigned long time = 0;
  //  time = millis();
  //timeは予約語なので変数利用禁止(多分)

  while (cycle(95.0) != 1) {
  }
  
  mytime = millis();
  while (millis() - mytime < 180000) {
    int key = cycle(95.0);
  }

  for (int i = 0; i < 30; i++) {
    while (cycle(95.0) != 1) {
    }

    mytime = millis();
    while (millis() - mytime < 15000) {
      int key = cycle(95.0);
    }
    while (cycle(59.0) != 1) {
    }

    mytime = millis();
    while (millis() - mytime < 45000) {
      int key = cycle(59.0);
    }
    Serial.println(i);
    ESP_BT.println(i);
  }

  while (1) {
    int key = cycle(15.0);
  }
}
