#include <Arduino.h>
// libraries para servidor Bluetooth
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

using namespace std;

int led = 13;

// definir a classe que processa a chegada de dados
// a forma como sao apresentados no serial monitor
// acende o LED a cada chegada de dados

class MyCallbacks: public BLECharacteristicCallbacks {
    int callbacksCheckPin;
  public:
    MyCallbacks (int initPin): callbacksCheckPin(initPin) {}
    void onWrite(BLECharacteristic *pCharacteristic) {
      string value = pCharacteristic->getValue();
      if (value.length() > 0) {
        digitalWrite(callbacksCheckPin, HIGH);
        Serial.print("\r\nNew value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);
        Serial.println();
        delay(500);
        digitalWrite(callbacksCheckPin, LOW);
      }
    }
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(led, OUTPUT);

  // iniciar o servidor na placa
  BLEDevice::init("ESP32 BLE example");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // setup das caracteristicas do servidor
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                        );

  pCharacteristic->setCallbacks(new MyCallbacks(led));

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Setup successfull. Ready to comunicate!");
  delay(2000);
  /*digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);*/

}
