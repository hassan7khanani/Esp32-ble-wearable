import socketio

# Socket.IO server URL
server_url = 'http://18.219.37.253:4000/'

# User ID
# user_id = '6196adbc3a2158526a8d3ad5'

# Create Socket.IO client instance
sio = socketio.Client()

# Event handlers
@sio.event
def connect():
    print('Connected to the server')
    # sio.emit('join', user_id)

@sio.event
def disconnect():
    print('Disconnected from the server')

@sio.event
def message(data):
    print('Received message:', data)

@sio.event
def error(data):
    print('Socket.IO error:', data)

# Connect to the server
sio.connect(server_url)

# Keep the connection open indefinitely
sio.wait()