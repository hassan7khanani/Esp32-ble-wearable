#include "Arduino.h"
#include <ctime>
#include <ArduinoJson.h>

long unix_time_ms;
String unix_time = "1687039546";
int rawDataLength, numChunks, startIndex;
String chunkData;
int hours, minutes, seconds;
unsigned long int milliseconds;
int sensorValue1A, sensorValue1B, sensorValue2A, sensorValue2B, sensorValue1, sensorValue2;
int unix_time_in_int;
int merged_unix_time;
String date;
int hourint, minint, secint;
char* dateTime ;
String sDateTime, timeString;
DynamicJsonDocument doc(1024);
JsonObject hourlyData[24]; // JSON objects for each hour
bool hourProcessed[24] = { false }; // Track whether we have started processing for each hour

struct SensorReading {
    int sensorValue1;
    int sensorValue2;
};

#define MAX_READINGS_PER_HOUR 100 
SensorReading sensorReadings[24][MAX_READINGS_PER_HOUR];
int sensorReadingCounts[24] = {0};

String rawData = "111111111111111106f1470301106f00071c389100006f000716f703430006f00024b070300006f00071b071800006f00071b070300006f24";

void HextoDec();
void setup() {
  Serial.begin(115200);
  delay(3000);
  unix_time_ms = unix_time.toInt();
  Serial.print("Received UNIX time is: ");
  Serial.println(unix_time_ms);
  milliseconds = (milliseconds) * 100;
  seconds = seconds ;
  minutes = minutes * 60 ;
  hours = hours * 60 * 60 ;
  unix_time_in_int = unix_time.toInt();
  HextoDec();
  printHourlyReadings();
}

void loop() {
  // No operations in the loop
}

void HextoDec() {
  doc["user"] = "JohnDoe";
  JsonArray data = doc.createNestedArray("data");
  rawDataLength = rawData.length();
  numChunks = rawDataLength / 16;
  
  for (int i = 0; i < numChunks; i++) {
    startIndex = i * 16;
    chunkData = rawData.substring(startIndex, startIndex + 16);
    // Parse values from rawData
    // Your parsing code goes here...

     hours = (byte) strtol(chunkData.substring(6, 8).c_str(), NULL, 16);
     minutes = (byte) strtol(chunkData.substring(4, 6).c_str(), NULL, 16);
     seconds = (byte) strtol(chunkData.substring(2, 4).c_str(), NULL, 16);
     milliseconds = (unsigned int) strtol(chunkData.substring(0, 2).c_str(), NULL, 16);

     sensorValue1A = (byte) strtol(chunkData.substring(8, 10).c_str(), NULL, 16);
     sensorValue1B = (byte) strtol(chunkData.substring(10, 12).c_str(), NULL, 16);
     sensorValue2A = (byte) strtol(chunkData.substring(12, 14).c_str(), NULL, 16);
     sensorValue2B = (byte) strtol(chunkData.substring(14, 16).c_str(), NULL, 16);
    
     sensorValue1 = sensorValue1A + sensorValue1B;
     sensorValue2 = sensorValue2A + sensorValue2B;
    
    // Print the parsed values for the current chunk
    Serial.println("Chunk " + String(i + 1) + ":");
    Serial.print("Hours: ");
    Serial.println(hours);
    Serial.print("Minutes: ");
    Serial.println(minutes);
    Serial.print("Seconds: ");
    Serial.println(seconds);
    Serial.print("Milliseconds: ");
    Serial.println(milliseconds);
    Serial.print("Sensor Value 1: ");
    Serial.println(sensorValue1);
    Serial.print("Sensor Value 2: ");
    Serial.println(sensorValue2);
    Serial.println();

    merged_unix_time=unix_time_in_int + hours + minutes + seconds ;
    Serial.print("New merged time:   ");
    Serial.println(merged_unix_time);
    time_t date_time = merged_unix_time;
    dateTime = ctime(&date_time);
    Serial.println(dateTime);
    String sDateTime = String(dateTime);
    String timeString = sDateTime.substring(11, 19); // Extract hours:minutes:seconds
    Serial.println(timeString);
    hourint = timeString.substring(0, 2).toInt();
    minint = timeString.substring(3, 5).toInt();
    secint = timeString.substring(6, 8).toInt();

    Serial.print("Hour: ");
    Serial.println(hourint);
    Serial.print("Minutes: ");
    Serial.println(minint);
    Serial.print("Seconds: ");
    Serial.println(secint);

    if (hourint < 17) {
      merged_unix_time -= 86400;  // Subtract one day
      Serial.print("after subtracting time:  ");
      Serial.println(merged_unix_time);
      time_t date_time = merged_unix_time;
      dateTime = ctime(&date_time);
      Serial.println(dateTime);
      String sDateTime = String(dateTime);
      String timeString = sDateTime.substring(11, 19); // Extract hours:minutes:seconds
      Serial.println(timeString);
      hourint = timeString.substring(0, 2).toInt();
      minint = timeString.substring(3, 5).toInt();
      secint = timeString.substring(6, 8).toInt();
      Serial.print("Hour: ");
      Serial.println(hourint);
      Serial.print("Minutes: ");
      Serial.println(minint);
      Serial.print("Seconds: ");
      Serial.println(secint);
    }
    // Add the sensor readings to the appropriate hour
    SensorReading reading;
    reading.sensorValue1 = sensorValue1;
    reading.sensorValue2 = sensorValue2;
        
    int hourIndex = hourint; // Assuming hourint is in range 0-23
    int readingIndex = sensorReadingCounts[hourIndex];
        
    // Check if we have space for more readings this hour
    if (readingIndex < MAX_READINGS_PER_HOUR) {
        sensorReadings[hourIndex][readingIndex] = reading;
        sensorReadingCounts[hourIndex]++;
    } else {
        Serial.println("Warning: reached maximum number of readings for hour " + String(hourIndex));
    }
    if (!hourProcessed[hourint]) {
        // We haven't seen this hour before, create a new JSON object
        hourlyData[hourint] = data.createNestedObject();
        sDateTime.trim(); 
        hourlyData[hourint]["ts"] = dateTime;
        hourlyData[hourint].createNestedArray("gr");
        hourProcessed[hourint] = true;
    }

    // Add the sensor readings to the array for this hour
    JsonObject sensorData = hourlyData[hourint]["gr"].createNestedObject();
    sensorData["sensor1"] = sensorValue1;
    sensorData["sensor2"] = sensorValue2;
  }
  Serial.println();
  serializeJson(doc, Serial);
  Serial.println();
}

void printHourlyReadings() {
  for (int hour = 0; hour < 24; hour++) {
    int readingsForHour = sensorReadingCounts[hour];
    Serial.println("Hour " + String(hour) + ": " + String(readingsForHour) + " readings");
    for (int i = 0; i < readingsForHour; i++) {
      SensorReading reading = sensorReadings[hour][i];
      Serial.println("  Reading " + String(i + 1) + ": Sensor Value 1 = " + String(reading.sensorValue1) + ", Sensor Value 2 = " + String(reading.sensorValue2));
    }
  }
}