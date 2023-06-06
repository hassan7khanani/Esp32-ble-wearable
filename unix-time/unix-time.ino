#include <ctime>

void setup() {
  // Begin serial communication at 9600 baud
  Serial.begin(115200);

  // UNIX timestamp
  time_t unixTime = 1685721561;
  Serial.println(typeof(unixTime));
  // Convert the UNIX timestamp to a string
  char* dateTime = ctime(&unixTime);
  // Print the date/time string
  Serial.println(dateTime);

 




  
}

void loop() {
  // No need to do anything here
}
