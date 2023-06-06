#include "Arduino.h"
#include <TimeLib.h>

long unix_time_ms;
String unix_time="1685606925";

String hexData = "071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00";
String new_data;
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
int dataCount ;
String hexReading;
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
  new_data[dataCount];
  for (int i = 0; i < dataCount; i++) {
    hexReading = hexData.substring(i * 16, (i + 1) * 16);
    Serial.print("Hex readings:   ");
    Serial.println(hexReading);
    new_data[i] = hexToDec(hexReading);
    Serial.print("Data after conversion is ");
    Serial.println(new_data[i]);
  }


  // Serial.println(new_data);

  for(int i = 0; i < dataCount; i++) {
    String ms = new_data[i].substring(0, 2);
    String s = new_data[i].substring(2, 4);
    String m = new_data[i].substring(4, 6);
    String h = new_data[i].substring(6, 8);

    long total_ms = ms.toInt() * 100;
    total_ms += s.toInt() * 1000;
    total_ms += m.toInt() * 60 * 1000;
    total_ms += h.toInt() * 60 * 60 * 1000;

    // Save total_ms back to data[i]
    new_data[i] = String(total_ms);

    // Add reversed value to unix_time_ms
    long new_unix_time_ms = unix_time_ms + new_data[i].toInt();
    time_t new_unix_time = new_unix_time_ms / 1000;  // Convert to seconds

    // Convert to hr:min:ss dd:mm:year
    tmElements_t tm;
    breakTime(new_unix_time, tm);

    // If Hour is less than 17, subtract one day (in seconds)
    if (tm.Hour < 17) {
      new_unix_time_ms -= SECS_PER_DAY * 1000L;  // Subtract one day
      new_unix_time = new_unix_time_ms / 1000;  // Convert back to seconds
      breakTime(new_unix_time, tm);  // Convert the updated UNIX time
    }

    // Create a String to represent the date
    String date = String(tmYearToCalendar(tm.Year)) + String(tm.Month) + String(tm.Day);

    // Check if the date already exists in the 'days' array
    int j;
    for (j = 0; j < days_count; j++) {
      if (days[j] == date) {
        break;
      }
    }

    // If the date does not exist, add it to the 'days' array
    if (j == days_count && days_count < MAX_DAYS) {
      days[days_count] = date;
      days_count++;
    }

    // Add the reading to the 'same_day' array
    if (j < MAX_DAYS && same_day_index[j] < MAX_READINGS_PER_DAY) {
      same_day[j][same_day_index[j]] = new_data[i];
      same_day_index[j]++;
    }

    // Add the reading to the 'same_hour' array
    if (tm.Hour < MAX_HOURS && same_hour_index[tm.Hour] < MAX_READINGS_PER_HOUR) {
      same_hour[tm.Hour][same_hour_index[tm.Hour]] = new_data[i];
      same_hour_index[tm.Hour]++;
    }
  }

  // Print the 'same_day' arrays
  for (int i = 0; i < days_count; i++) {
    Serial.println("Data on day " + days[i] + ":");
    for (int j = 0; j < same_day_index[i]; j++) {
      Serial.println(same_day[i][j]);
    }
  }

  // Print the 'same_hour' arrays
  for (int i = 0; i < MAX_HOURS; i++) {
    if (same_hour_index[i] > 0) {
      Serial.println("Data in hour " + String(i) + ":");
      for (int j = 0; j < same_hour_index[i]; j++) {
        Serial.println(same_hour[i][j]);
      }
    }
  }




  // Rest of the setup code...

}

void loop() {
  // No operations in the loop
}
