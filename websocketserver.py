# # import asyncio
# # import websockets
# # import json

# # async def send_message(websocket, topic, message):
# #     data = {
# #         "topic": topic,
# #         "message": message
# #     }
# #     payload = json.dumps(data)
# #     await websocket.send(payload)
# #     print(f"Sent message: {payload}")

# # async def handle_client(websocket, path):
# #     while True:
# #         await asyncio.sleep(1)  # Wait for 1 second
        
# #         topic = "grind"
# #         message = "Hello from server"

# #         await send_message(websocket, topic, message)

# # async def start_server():
# #     server = await websockets.serve(handle_client, "192.168.100.50", 80)

# #     print("WebSocket server started")
# #     await server.wait_closed()

# # asyncio.get_event_loop().run_until_complete(start_server())

# import asyncio
# import websockets
# import json

# async def send_message(websocket, topic, message):
#     data = {
#         "topic": topic,
#         "message": message
#     }
#     payload = json.dumps(data)
#     try:
#         # print("client connected")
#         await websocket.send(payload)
#         # print(f"Sent message: {payload}")
#     except websockets.exceptions.ConnectionClosed:
#         print("Client is not connected")

# async def handle_client(websocket, path):
#     while True:
#         await asyncio.sleep(1)  # Wait for 1 second

#         topic = "grind"
#         message = "Hello from server"

#         await send_message(websocket, topic, message)

# async def start_server():
#     server = await websockets.serve(handle_client, "192.168.100.50", 80)

#     print("WebSocket server started")
#     await server.wait_closed()

# asyncio.get_event_loop().run_until_complete(start_server())
# # import asyncio
# # import websockets
# # import json

# # async def send_message(websocket, topic, message):
# #     data = {
# #         "topic": topic,
# #         "message": message
# #     }
# #     payload = json.dumps(data)
# #     try:
# #         await websocket.send(payload)
# #         print(f"Sent message: {payload}")
# #     except websockets.exceptions.ConnectionClosed:
# #         print("Client is not connected")

# # async def handle_client(websocket, path):
# #     async for msg in websocket:
# #         data = json.loads(msg)
# #         if 'topic' in data and data['topic'] == 'grindchannel':
# #             print(f"Received message on grindchannel: {data['message']}")

# #     while True:
# #         await asyncio.sleep(1)  # Wait for 1 second

# #         topic = "grind"
# #         message = "Hello from server"

# #         await send_message(websocket, topic, message)

# # async def start_server():
# #     server = await websockets.serve(handle_client, "192.168.100.50", 80)

# #     print("WebSocket server started")
# #     await server.wait_closed()

# # asyncio.get_event_loop().run_until_complete(start_server())


import asyncio
import websockets

async def echo(websocket, path):
    print("Client connected")
    async for message in websocket:
        print("Received Message: {}".format(message))
8
start_server = websockets.serve(echo, "192.168.100.50", 80)


asyncio.get_event_loop().run_until_complete(start_server)
print("Server started")
asyncio.get_event_loop().run_forever()
