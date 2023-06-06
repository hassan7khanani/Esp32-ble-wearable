/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */
#define RESET_C "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"

#include "BLEDevice.h"
//#include "BLEScan.h"
#include <BLEServer.h>
#include <BLEUtils.h>
#include <string.h>

// // The remote service we wish to connect to.
// static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// // The characteristic of the remote service we are interested in.
// static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

String responseValue1 = "";
String newVal1 = "fram,1,08\n";
String newVal2 = "data_sync,"+ responseValue1 +"\n";
String newVal3 = "fram,X,Y(,Z)\n";
String newVal4 = "battery\n";
String newVal5 = "reboot\n";

bool sent1 = false;
bool sent2 = false;
bool sent3 = false;
bool sent4 = false;
bool sent5 = false;

// The remote service we wish to connect to.
static BLEUUID serviceUUID ("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
static BLEUUID charUUID ("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");//TX 
static BLEUUID charUUID2 ("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");//RX

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristicTX;
static BLERemoteCharacteristic* pRemoteCharacteristicRX;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback ");
    Serial.print(RED);
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(RESET_C);
    Serial.print(" of data length ");
    Serial.print(RED);
    Serial.println(length);
    Serial.print(RESET_C);
    Serial.println();
    Serial.println();
    Serial.print("data: ");
    Serial.print(BLUE);
    Serial.write(pData, length);
    Serial.println();

//BLE TX EVENT FROM PERIPHERAL

    if(sent3 == true){
        
        responseValue1 = pRemoteCharacteristicTX->readValue().c_str(); //UNIX device start timestamp receiving
        sent3 = false;
    }

    if(sent4 == true){

        String responseValue2 = pRemoteCharacteristicTX->readValue().c_str(); // data_sync string receiving
      
        if(responseValue2 != "1,A,B,C,D,E,F,G,H"){ 
            const char delimiter[] = ","; 
            const int maxTokens = 10; // Maximum number of tokens in the string

            char* tokens[maxTokens]; // Array to store the tokens

            int tokenCount = 0;
            char* responseCopy = strdup(responseValue2.c_str());  // Make a copy of the string
            char* token = strtok(responseCopy,delimiter);

            while (token != NULL && tokenCount < maxTokens) {
              tokens[tokenCount] = token;
              Serial.println("TOKEN: ");
              Serial.println(token);
              tokenCount++;
              token = strtok(NULL, delimiter);
            }
            Serial.println("Token count: ");
            Serial.println(tokenCount);

            free(responseCopy);  // Free the memory allocated by strdup
            // Ensure that the expected number of tokens were found
            if (tokenCount == maxTokens) {
              int Hour = atoi(tokens[1]);     // Convert token to integer
              int Minutes = atoi(tokens[2]);  // Convert token to integer
              int Seconds = atoi(tokens[3]);  // Convert token to integer
              int Millisec = atoi(tokens[4]);     // Convert token to integer
              int SEN1_A = atoi(tokens[5]);  // Convert token to integer
              int SEN1_B = atoi(tokens[6]);  // Convert token to integer
              int SEN2_A = atoi(tokens[7]);     // Convert token to integer
              int SEN2_B = atoi(tokens[8]);  // Convert token to integer

              // Print the values
              Serial.println(GREEN);
              Serial.print("Hour: "); Serial.println(Hour);
              Serial.print("Minutes: "); Serial.println(Minutes);
              Serial.print("Seconds: "); Serial.println(Seconds);
              Serial.print("Millisec: "); Serial.println(Millisec);
              Serial.print("SEN1_A: "); Serial.println(SEN1_A);
              Serial.print("SEN1_B: "); Serial.println(SEN1_B);
              Serial.print("SEN2_A: "); Serial.println(SEN2_A);
              Serial.print("SEN2_B: "); Serial.println(SEN2_B);
              
              String concatenatedValues;
              for (int i = 1; i < 5; i++) { // Start from index 1 to skip the first token
                concatenatedValues += tokens[i];
              }
              
              // Print the concatenated values
              Serial.print("Concatenated Values: ");
              Serial.println(concatenatedValues);
              
              int concatenatedInt = concatenatedValues.toInt(); // Convert the concatenated string to an integer
              
              // Print the concatenated integer
              Serial.print("Concatenated Integer: ");
              Serial.println(concatenatedInt); 

              int responseValueInt = responseValue1.toInt();
              uint32_t add_time = concatenatedInt + responseValueInt;
              Serial.print("ADDED TIME:");
              Serial.println(GREEN);
              Serial.println(add_time);

        } else {
              Serial.println(GREEN);
              Serial.println("Invalid number of tokens");
            }
        } sent4 = false;
    }
    if(sent1 == true){

        String responseValue3 = pRemoteCharacteristicTX->readValue().c_str();
        sent3 = false;
    }

    if(sent5 == true){

        String responseValue4 = pRemoteCharacteristicTX->readValue().c_str(); //battery value receiving

        if(responseValue4 != "1,A,B,C,D,E,F,G,H"){  
            int responseValue4Int = responseValue4.toInt();
            float voltage = (responseValue4Int/4095.0)*7.2;
            Serial.print("The voltage val: ");
            Serial.println(GREEN);
            Serial.println(voltage);
            pinMode(2, OUTPUT);

            if(voltage >= 4.3){
              Serial.println(GREEN);
              Serial.println("GPIO LOW BCOZ 4.3 FULLY CHARGE");
              digitalWrite(2, LOW);
            }else{
              Serial.println(GREEN);
              Serial.println("GPIO HIGH BCOZ NOT CHARGE");
              digitalWrite(2, HIGH);
              delay(1000);
              digitalWrite(2, LOW);
            }
        } sent5 = false;
    }

    if(sent2 == true){
        String responseValue5 = pRemoteCharacteristicTX->readValue().c_str();
        sent2 = false;
    }
//BLE CLIENT HANDLING DONE

}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(RED);
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
  
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.println(RED);
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(RED);
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristicTX = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristicTX == nullptr) {
      Serial.println(BLUE);
      Serial.print("Failed to find TX characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(BLUE);
    Serial.println(" - Found our characteristic TX");

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristicRX = pRemoteService->getCharacteristic(charUUID2);
    if (pRemoteCharacteristicRX == nullptr) {
      Serial.println(BLUE);
      Serial.print("Failed to find RX characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(BLUE);
    Serial.println(" - Found our characteristic RX");

    // Read the value of the characteristic.
    if(pRemoteCharacteristicTX->canRead()) {
      std::string value = pRemoteCharacteristicTX->readValue();
      Serial.println(BLUE);
      Serial.print("The TX characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristicTX->canNotify()){
      pRemoteCharacteristicTX->registerForNotify(notifyCallback);

    connected = true;
    return true;}
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.println(BLUE);
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println(BLUE);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.

void reboot_fn(){
  Serial.println(RED);
  Serial.println("=== Performs a Soft reset of wearable ===");
  pRemoteCharacteristicRX->writeValue(newVal1.c_str(), newVal1.length());
  sent1 = true;
}

void framXYZ_fn(){
  Serial.println(RED);
  Serial.println("=== Read or Write to Wearable location ===");
  pRemoteCharacteristicRX->writeValue(newVal2.c_str(), newVal2.length());
  sent2 = true;
}

void framUNIX_fn(){
  Serial.println(RED);
  Serial.println("=== Returns UNIX Time of wearable device ===");
  pRemoteCharacteristicRX->writeValue(newVal3.c_str(), newVal3.length());
  sent3 = true;
}

void datasync_fn(){
  Serial.println(RED);
  Serial.println("=== Sync data of wearable and store in EEPROM ===");
  pRemoteCharacteristicRX->writeValue(newVal4.c_str(), newVal4.length());
  sent4 = true;
}

void battery_fn(){
  Serial.println(RED);
  Serial.println("=== Find battery voltage of wearable ===");
  pRemoteCharacteristicRX->writeValue(newVal5.c_str(), newVal5.length());
  sent5 = true;
}

void loop() {

  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println(BLUE);
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println(BLUE);
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  if (connected) {

    Serial.println(GREEN);
    Serial.println("--- IN LOOP: CENTRAL CONNECTED ---");

    reboot_fn();
    Serial.print("FUNCTION 1 EXIT");
    delay(2000);
    framXYZ_fn();
    Serial.print("FUNCTION 2 EXIT");
    delay(2000);
    framUNIX_fn();
    Serial.print("FUNCTION 3 EXIT");
    delay(2000);
    datasync_fn();
    Serial.print("FUNCTION 4 EXIT");
    delay(2000);
    battery_fn();
    Serial.print("FUNCTION 5 EXIT");
    delay(2000);
    
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  //delay(1000); // Delay a second between loops.
} // End of loop
