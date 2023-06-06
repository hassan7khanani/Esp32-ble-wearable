
import websocket
import time

# WebSocket server URL
server_url = "ws://18.219.37.253:4000/"

# Flag variable to track the connection status
connected = False

# Function to handle the WebSocket connection open event
def on_open(ws):
    global connected
    connected = True
    print("Connection to ESP32 WebSocket server successful.")
    channel = "esp32"
    message = "Hello ESP32!"
    ws.send(f"{channel}:{message}")
    print("Message sent on esp32 channel")
    # time.sleep(2)
    # Subscribe to the "esp32pub" channel
    channel = "esp32pub"
    subscribe_message = f"subscribe:{channel}"
    ws.send(subscribe_message)
    print(f"Subscribed to the {channel} channel.")

# Function to handle incoming messages
def on_message(ws, message):
    print("Received message:", message)

# Connect to the WebSocket server
ws = websocket.WebSocketApp(server_url, on_message=on_message, on_open=on_open)
ws.run_forever()

# Check the connection status
if connected:
    print("Connection to ESP32 WebSocket server successful.")
else:
    print("Connection to ESP32 WebSocket server failed.")

# Close the WebSocket connection
# ws.close()
