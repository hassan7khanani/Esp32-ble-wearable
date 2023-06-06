#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char* ssid = "Virus";
const char* password = "12345678";
const int webSocketPort = 8080;

WebSocketsServer webSocket = WebSocketsServer(webSocketPort);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_TEXT: {
      // Split the message into channel and payload
      String message = String((char*)payload);
      int separatorIndex = message.indexOf(":");
      if (separatorIndex != -1) {
        String channel = message.substring(0, separatorIndex);
        String payload = message.substring(separatorIndex + 1);

        // Check if the message is on the "esp32" channel
        if (channel == "esp32") {
          Serial.print("Received message on esp32 channel: ");
          Serial.println(payload);

          // Create a JSON message to emit on the "esp32pub" channel
          StaticJsonDocument<64> jsonDoc;
          jsonDoc["data"] = "hello";

          // Convert the JSON message to a string
          String jsonString;
          serializeJson(jsonDoc, jsonString);

          // Emit the JSON message on the "esp32pub" channel
          webSocket.broadcastTXT("esp32pub:" + jsonString);
          Serial.println("message sent");
        }
      }
      break;
    }
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(5000);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print the ESP32 IP address
  Serial.print("WebSocket server IP address: ");
  Serial.println(WiFi.localIP());

  // Start the WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();
}
