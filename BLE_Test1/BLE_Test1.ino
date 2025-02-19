/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second. 
*/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
bool lastDeviceConnectedState = false;
float txValue = 0;
const int LED = 2; // Could be different depending on the dev board. I used the DOIT ESP32 dev board.
String rxValue;

//std::string rxValue; // Could also make this a global var to access it in loop()

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "7ac6d961-1f73-4241-9df7-362beb29b70d"
#define CHARACTERISTIC_UUID_TX "d53c9aca-0642-495c-b07b-c5ea47ee23df"

BLEServer *pServer;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        
        // For Debugging
        Serial.println("*********");
        Serial.print("Received Value: ");
        Serial.println(rxValue);
        Serial.println("---------");

        // Do Stuff Here
        if (rxValue == "LED1.ON") { 
          Serial.println("Turning ON!");
          digitalWrite(LED, HIGH);
        } else if (rxValue == "LED1.OFF") {
          Serial.println("Turning OFF!");
          digitalWrite(LED, LOW);
        }
      }
    }
};

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  // Create the BLE Device
  BLEDevice::init("ESP32_ARMM_BLE"); // Give it a name

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  if (deviceConnected) { // Run when a device is connected....
    
    // Generate a random value
    txValue = analogRead(0);

    // Convert value to a string
    char txString[8];
    dtostrf(txValue, 1, 2, txString); // float_val, min_width, digits_after_decimal, char_buffer
    
    // Alternative tx methods
    // pCharacteristic->setValue(&txValue, 1); // To send the integer value
    // pCharacteristic->setValue("Hello!"); // Sending a test message

    // Transmit the string
    pCharacteristic->setValue(txString);
    
    // Notify a characteristic change
    pCharacteristic->notify();
    Serial.print("*** Sent Value: ");
    Serial.print(txString);
    Serial.println(" ***");
    
  /*
    You can add the rxValue checks down here instead
    if you set "rxValue" as a global var at the top!
    Note you will have to delete "std::string" declaration
    of "rxValue" in the callback function.
    if (rxValue == "LED1.ON") { 
      Serial.println("Turning ON!");
      digitalWrite(LED, HIGH);
    } else if (rxValue == "LED1.OFF") {
      Serial.println("Turning OFF!");
      digitalWrite(LED, LOW);
    }

  */

  } else if (lastDeviceConnectedState != deviceConnected) { // If there is not a device connected

    Serial.println();
    Serial.print("Lost Connection. Relaunching BLE service....   ");
    pServer->getAdvertising()->start();
    Serial.println("Relaunched!");
  }

  // Update last device state
  lastDeviceConnectedState = deviceConnected;

  delay(500);
}
