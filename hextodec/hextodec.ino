#include "Arduino.h"
#include <TimeLib.h>

long unix_time_ms;
String unix_time="1685606925";

String hexData = "071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00";

#define MAX_READINGS_PER_HOUR 10
#define MAX_HOURS 24
#define MAX_READINGS_PER_DAY 10
#define MAX_DAYS 365
String days[MAX_DAYS];  // to store different days
String same_day[MAX_DAYS][MAX_READINGS_PER_DAY]; // to hold readings for each day
String same_hour[MAX_HOURS][MAX_READINGS_PER_HOUR]; // to hold readings for each hour
int same_day_index[MAX_DAYS] = {0};  // to hold current index for each day
int same_hour_index[MAX_HOURS] = {0};  // to hold current index for each hour
int days_count = 0;  // to keep track of the number of different days
// int dataCount = hexData.length() / 16;  // Each reading is 8 bytes, but 2 characters represent 1 byte
// String new_data[dataCount];
// String hexReading;
// Helper function to convert a hex string to a decimal
String hexToDec(String hexString) {
  String result = "";
  for (int i = 0; i < hexString.length(); i += 2) {
    String byteString = hexString.substring(i, i + 2);
    unsigned int byte = strtol(byteString.c_str(), NULL, 16);
    result += String(byte) + " ";
  }
  return result;
}

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  delay(5000);
  
  // Convert the String to a long integer, multiply it by 1000 to get milliseconds
  unix_time_ms = unix_time.toInt() * 1000L;
  
  // Print the value to the serial monitor
  Serial.print("Received UNIX time is: ");
  Serial.println(unix_time_ms);

  // Calculate the number of readings in the hex data
  int dataCount = hexData.length() / 16;  // Each reading is 8 bytes, but 2 characters represent 1 byte

  // Convert the hex data to decimal and store them in the new data
  String new_data[dataCount];
  for (int i = 0; i < dataCount; i++) {
    String hexReading = hexData.substring(i * 16, (i + 1) * 16);
    Serial.print("Hex readings:   ");
    Serial.println(hexReading);
    new_data[i] = hexToDec(hexReading);
    Serial.print("Data after conversion is ");
    Serial.println(new_data[i]);
  }

  // Rest of the setup code...

}

void loop() {
  // No operations in the loop
}
