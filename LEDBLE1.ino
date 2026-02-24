#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcd1234-5678-1234-5678-abcdef123456"
#define LED_PIN 2   

bool dispositivoConectado = false;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    dispositivoConectado = true;
    Serial.println("📲 Conectado");
  }

  void onDisconnect(BLEServer* pServer) {
    dispositivoConectado = false;
    Serial.println("❌ Desconectado");
    BLEDevice::startAdvertising();
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {

    String palabra = pCharacteristic->getValue();
    palabra.trim();

    Serial.print("📩 Palabra recibida: ");
    Serial.println(palabra);

    if (palabra == "ON") {
      digitalWrite(LED_PIN, HIGH);
    }
    else if (palabra == "OFF") {
      digitalWrite(LED_PIN, LOW);
    }
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  BLEDevice::init("ESP32_BLE");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();

  BLEDevice::startAdvertising();

  Serial.println("📡 ESP32 listo");
}

void loop() {
}
