#include "Arduino.h"
// #include <TimeLib.h>
#include <ctime>

long unix_time_ms;
String unix_time="1685606925";


unsigned int a=1685606925;
String hexData = "071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00";
int rawDataLength,numChunks,startIndex;

String chunkData;
int hours,minutes,seconds;
unsigned long int milliseconds;
int sensorValue1A,sensorValue1B,sensorValue2A,sensorValue2B,sensorValue1,sensorValue2;
int unix_time_in_int;
int merged_unix_time;
String date;
int time_1;
#define MAX_READINGS_PER_HOUR 10
#define MAX_HOURS 24
#define MAX_READINGS_PER_DAY 10
#define MAX_DAYS 365
#define MAX_DATA 256  // Ensure this number is large enough to hold your data
String days[MAX_DAYS];  // to store different days
String same_day[MAX_DAYS][MAX_READINGS_PER_DAY]; // to hold readings for each day
String same_hour[MAX_HOURS][MAX_READINGS_PER_HOUR]; // to hold readings for each hour
int same_day_index[MAX_DAYS] = {0};  // to hold current index for each day
int same_hour_index[MAX_HOURS] = {0};  // to hold current index for each hour
int days_count = 0;  // to keep track of the number of different days
String converted_data[MAX_DATA];  // array to hold converted data

String rawData = "071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00071b070300006f00";



void setup() {
  // Start the serial communication
  Serial.begin(115200);
  delay(3000);

  unix_time_ms = unix_time.toInt() ;
  a=a*1000;
  Serial.print("a value ");
  Serial.println(a);  
  // Print the value to the serial monitor
  Serial.print("Received UNIX time is: ");
  Serial.println(unix_time_ms);
  HextoDec();
   

    milliseconds = (milliseconds) * 100;
    seconds = seconds ;
    minutes = minutes * 60 ;
    hours = hours * 60 * 60 ;

    Serial.print("new ms value ");
    Serial.println(milliseconds);
    Serial.print("new seconds value ");
    Serial.println(seconds);
    Serial.print("new min value ");
    Serial.println(minutes);
    Serial.print("new hour value ");
    Serial.println(hours);

    unix_time_in_int=unix_time.toInt();

    Serial.print("new unix in int :");
    Serial.println(unix_time_in_int);

    // time_1=hours+minutes+seconds+;
    // Serial.print("sum of time is    ");
    // Serial.println(time_1);    



    merged_unix_time=unix_time_in_int + hours + minutes + seconds ;
    Serial.print("New merged time:   ");
    Serial.println(merged_unix_time);
    time_t date_time = merged_unix_time;
    char* dateTime = ctime(&date_time);
    Serial.println(dateTime);
    String sDateTime = String(dateTime);
    String timeString = sDateTime.substring(11, 19); // Extract hours:minutes:seconds
    Serial.println(timeString);
    int hourString = timeString.substring(0, 2).toInt();
    int minString = timeString.substring(3, 5).toInt();
    int secString = timeString.substring(6, 8).toInt();

    Serial.print("Hour: ");
    Serial.println(hourString);
    Serial.print("Minutes: ");
    Serial.println(minString);
    Serial.print("Seconds: ");
    Serial.println(secString);
    

    // Serial.println(dateTime.Hour);



  //   // Convert to hr:min:ss dd:mm:year
    // tmElements_t tm;
    // breakTime(merged_unix_time, tm);
    //     // Create a String to represent the date
    // date = String(tmYearToCalendar(tm.Year))+" " + String(tm.Month) +" "+ String(tm.Day);
    // Serial.print("Date is ");
    // Serial.println(date);
    // Serial.print("hour is ");
    // Serial.println(tm.Hour);



    // if (tm.Hour < 17) {
    //   merged_unix_time -= SECS_PER_DAY * 1000L;  // Subtract one day
    //   merged_unix_time = merged_unix_time / 1000;  // Convert back to seconds
    //   breakTime(merged_unix_time, tm);  // Convert the updated UNIX time
    // }

    // Save total_ms back to data[i]
    // converted_data[i] = String(total_ms);
    
    // Add reversed value to unix_time_ms
  //   long new_unix_time_ms = unix_time_ms + converted_data[i].toInt();
  //   time_t new_unix_time = new_unix_time_ms / 1000;  // Convert to seconds

  //   // Convert to hr:min:ss dd:mm:year
  //   tmElements_t tm;
  //   breakTime(new_unix_time, tm);

  //   // If Hour is less than 17, subtract one day (in seconds)
  //   if (tm.Hour < 17) {
  //     new_unix_time_ms -= SECS_PER_DAY * 1000L;  // Subtract one day
  //     new_unix_time = new_unix_time_ms / 1000;  // Convert back to seconds
  //     breakTime(new_unix_time, tm);  // Convert the updated UNIX time
  //   }

  //   // Create a String to represent the date
  //   String date = String(tmYearToCalendar(tm.Year)) + String(tm.Month) + String(tm.Day);

  //   // Check if the date already exists in the 'days' array
  //   int j;
  //   for (j = 0; j < days_count; j++) {
  //     if (days[j] == date) {
  //       break;
  //     }
  //   }

  //   // If the date does not exist, add it to the 'days' array
  //   if (j == days_count && days_count < MAX_DAYS) {
  //     days[days_count] = date;
  //     days_count++;
  //   }

  //   // Add the reading to the 'same_day' array
  //   if (j < MAX_DAYS && same_day_index[j] < MAX_READINGS_PER_DAY) {
  //     same_day[j][same_day_index[j]] = converted_data[i];
  //     same_day_index[j]++;
  //   }

  //   // Add the reading to the 'same_hour' array
  //   if (tm.Hour < MAX_HOURS && same_hour_index[tm.Hour] < MAX_READINGS_PER_HOUR) {
  //     same_hour[tm.Hour][same_hour_index[tm.Hour]] = converted_data[i];
  //     same_hour_index[tm.Hour]++;
    
  // }

  // // Print the 'same_day' arrays
  // for (int i = 0; i < days_count; i++) {
  //   Serial.println("Data on day " + days[i] + ":");
  //   for (int j = 0; j < same_day_index[i]; j++) {
  //     Serial.println(same_day[i][j]);
  //   }
  // }

  // // Print the 'same_hour' arrays
  // for (int i = 0; i < MAX_HOURS; i++) {
  //   if (same_hour_index[i] > 0) {
  //     Serial.println("Data in hour " + String(i) + ":");
  //     for (int j = 0; j < same_hour_index[i]; j++) {
  //       Serial.println(same_hour[i][j]);
  //     }
  //   }
  // }


}

void loop() {
  // No operations in the loop
}




void HextoDec()
{
   rawDataLength = rawData.length();
   numChunks = rawDataLength / 16;  // Assuming each chunk is 16 characters (8 bytes)
  
  for (int i = 0; i < numChunks; i++) {
    int startIndex = i * 16;
     chunkData = rawData.substring(startIndex, startIndex + 16);
    
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
  }
}