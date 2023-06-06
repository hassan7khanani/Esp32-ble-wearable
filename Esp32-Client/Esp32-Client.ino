#include <WiFi.h>
#include <WebSocketsClient.h>

// Wifi credentials
const char* ssid = "Virus";
const char* password = "12345678";

// Websocket server
const char* websocket_server_host = "192.168.100.50";
int websocket_server_port = 80;  // replace with your server port

WebSocketsClient webSocket;

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to WebSocket server
  webSocket.begin(websocket_server_host, websocket_server_port);
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();

  if (webSocket.isConnected()) {
    // Format and send message
    String channel = "grindRatio";
    String message = "{\"timestamp\":\"10234512\"}";
    webSocket.sendTXT(channel + ": " + message);
    delay(10000); // Delay for 10 seconds before sending the next message
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);
      break;
  }
}
 
