/*LED ON/OFF from ESP32 using Classic Bluetooth*/

#include "BluetoothSerial.h" 

BluetoothSerial ESP_BT; 

int incoming; //Incoming data from Smart Phone
int LED_1 = 26;
int LED_2 = 27;

void setup() {
  Serial.begin(115200); //Start Serial monitor in 115200
  ESP_BT.begin("ESP32_LED_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

  pinMode (LED_1, OUTPUT);
  pinMode (LED_2, OUTPUT);
}

void loop() {

  if (ESP_BT.available()) //Check if we receive anything from Bluetooth
  {
    incoming = ESP_BT.read(); //Read what we recevive
    Serial.print("Received:"); Serial.println(incoming);

    if (incoming == 49) //1→49(ASCII chart)
        {
        digitalWrite(LED_1, HIGH);
        ESP_BT.println("LED_1 turned ON");
        }
        

    if (incoming == 48) //0→48(ASCII chart)
        {
        digitalWrite(LED_1, LOW);
        ESP_BT.println("LED_1 turned OFF");
        }

    if (incoming == 51) //3→51(ASCII chart)
        {
        digitalWrite(LED_2, HIGH);
        ESP_BT.println("LED_2 turned ON");
        }

    if (incoming == 50) //2→50(ASCII chart)
        {
        digitalWrite(LED_2, LOW);
        ESP_BT.println("LED_2 turned OFF");
        }

  }
  delay(20);
}
