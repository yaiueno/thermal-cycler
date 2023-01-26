//山形県立米沢興譲館高等学校r04年度２年生上野能登制作
#include <OneWire.h>
#include "Arduino.h"
#include <DallasTemperature.h>
#include "BluetoothSerial.h" //ライブラリ類

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 25

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
BluetoothSerial ESP_BT;//btオブジェクトの定義
float temp=0;

void setup(void)
{
  unsigned long time[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  float temp[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  Serial.begin(115200);
  ESP_BT.begin("ESP32ueno"); //btの検索ででてくる名前
  delay(100);
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
  // start serial port
  pinMode (32, OUTPUT);
  pinMode (33, OUTPUT);
  pinMode (26, OUTPUT);
  pinMode (27, OUTPUT);
  delay(100);
  Serial.println("準備完了");
  sensors.begin();//温度センサー開始
  while(1)
  {
    getdata("温度（半角数字入力で終わりに;入力）");
    getdata("時間（半角数字入力で終わりに;を入力）");
    getdata("終了しますか？終了する場合は0を入力");
  }
}

String getdata(String message)//設定読み取り用関数、messageのなかにはユーザーに伝えたい言葉を入れる
{
    Serial.println(message);
    ESP_BT.println(message);
    String data;
    while(1)
  {
    if (Serial.available())
    {
    String data = Serial.readStringUntil(';');
    delay(100);
    break;
    }
    else if (ESP_BT.available())
    {
    String data = ESP_BT.readStringUntil(';');
    delay(100);
    break;
    }
  }
  return(data);
}

int cycle(float temp)//サイクル用関数
{
  int key=0;
  sensors.requestTemperatures(); // 温度センサーに温度をリクエスト
  Serial.println("実行");
  ESP_BT.println("実行");
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));
  ESP_BT.println(sensors.getTempCByIndex(0));
  float check=abs(sensors.getTempCByIndex(0)-temp);
  if(check<1.0)//理想との温度差が一度以下だったときの処理何もしない
  {
    digitalWrite(32, LOW);
    digitalWrite(33, LOW);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    key=1;//これで目標温度に達したか見る
    Serial.println("適温");
    ESP_BT.println("適温");
  }
  else if(sensors.getTempCByIndex(0)<temp)//理想よりも低い温める
  {
    digitalWrite(32, HIGH);
    digitalWrite(33, LOW);
    digitalWrite(26, HIGH);
    digitalWrite(27, HIGH);
    Serial.println("低い");
    ESP_BT.println("低い");
  }
  else if(sensors.getTempCByIndex(0)>temp)//理想よりも高い冷やす
  {
    digitalWrite(32, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    Serial.println("高い");
    ESP_BT.println("高い");
  }
  return(key);
  delay(10);
}

void exe(float temp,unsigned long ms)//実行用関数、温度安定化→目標秒数キープまでやってくれる
{
  unsigned long time=0;
  time=millis();
  while(1)
  {
    int key=cycle(temp);
    if(key==1)
    {
     break; 
    }
  }
  time=millis();
  while(1)
  {
    int key=cycle(temp);
    if(millis()-time>ms)
    {
     break; 
    }
  }
}

void finish(float temp)//サイクルが終わった後の温度設定
{
    while(1)
  {
    int key=cycle(temp);
  }
}

void loop(void)
{ 
  exe(70.0,1000);
  finish(60.0);
}
