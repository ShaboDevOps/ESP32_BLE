#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Define UUIDs for services and characteristics
#define SERVICE_UUID        "0000180d-0000-1000-8000-00805f9b34fc"
#define CHARACTERISTIC_UUID "00002a37-0000-1000-8000-00805f9b34fc"

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Device connected");
    }
    
    void onDisconnect(BLEServer* pServer) {
      Serial.println("Device disconnected");
    }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        String inputData, username, password;
        for (int i = 0; i < value.length(); i++) {

          inputData = inputData + value[i];

        }
        int spaceIndex = inputData.indexOf(' '); // Find the index of the space character
        if (spaceIndex != -1) { // If space character is found
          username = inputData.substring(0, spaceIndex); // Extract the first word
          password = inputData.substring(spaceIndex + 1); // Extract the second word
        } else {
          Serial.println("No space Found in" + inputData);
        }
        Serial.println("Username: " + username);
        Serial.println("Password: " + password);
        Serial.println();

        // Handle the received data, e.g., save WiFi credentials, etc.
      }
    }

    void onRead(BLECharacteristic* pCharacteristic) {
      // Implement read operation if needed
    }
};

void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("BLE Device");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Create the BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  // Add a descriptor for the characteristic
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
}

void loop() {
  // Other loop tasks
}
