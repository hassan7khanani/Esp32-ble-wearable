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

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
//uint16_t txValue = 2012;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


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
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Function: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);

        Serial.println();
        Serial.println("------");
      }

        if (rxValue == "battery\n"){
          Serial.println("Returning Battery Voltages");
          pTxCharacteristic->setValue("2011"); 
          pTxCharacteristic->notify();
        }
        if (rxValue == "reboot\n"){
          Serial.println("Rebooting wearable");
          pTxCharacteristic->setValue("performs a soft reset of the wearable"); 
          pTxCharacteristic->notify();
        }
        if (rxValue == "fram,1,08\n"){
          Serial.println("Sending unix timestamp");
          pTxCharacteristic->setValue("1684284581"); 
          pTxCharacteristic->notify();
        }
        if (rxValue == "data_sync,1684284581\n"){
        //if (rxValue.startsWith("data_sync,")){
          // pTxCharacteristic->setValue("1,7343,2143,26,110,7333,0,0,0,1\n"); 
          Serial.println("Returning time and sensor values 8 bytes");
          pTxCharacteristic->setValue("1,2,3,44,110,73,33,22,44,1\n"); 
          pTxCharacteristic->notify();
        }
        if (rxValue == "fram,X,Y(,Z)\n"){
          Serial.println("Fram called");
          pTxCharacteristic->setValue("I DON'T KNOW WHAT TO DO\n"); 
          pTxCharacteristic->notify();
        }
    }
};


void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("UART Service RX_TX");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY |
                    BLECharacteristic::PROPERTY_READ
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  //pServer->getAdvertising()->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();

  Serial.println("Waiting for central device ...");

  
    if (deviceConnected) {
        //pTxCharacteristic->setValue(&txValue, 1); “1,A,B,C,D,E,F,G,H”
        pTxCharacteristic->setValue("1,A,B,C,D,E,F,G,H"); 
        pTxCharacteristic->notify();
        //txValue++;
		delay(500); // bluetooth stack will go into congestion, if too many packets are sent
	}
}

void loop() {

  //   if (deviceConnected) {
  //       //pTxCharacteristic->setValue(&txValue, 1); “1,A,B,C,D,E,F,G,H”
  //       pTxCharacteristic->setValue("1,A,B,C,D,E,F,G,H"); 
  //       pTxCharacteristic->notify();
  //       //txValue++;
	// 	delay(5000); // bluetooth stack will go into congestion, if too many packets are sent
	// }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
		// do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}
