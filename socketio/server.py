import eventlet
import socketio
import numpy as np
import cv2

sio = socketio.Server()
app = socketio.WSGIApp(sio, static_files={
    '/': {'content_type': 'text/html', 'filename': 'index.html'}
})

@sio.on('connect')
def connect(sid, environ):
    print('>>> connect ', sid)

@sio.on('ch1')
def on_message(sid, data):
    print('>>> \tmessage')
    img = np.frombuffer(data, dtype=np.uint8).reshape((480, 640, 4))
    sio.emit('ch1', b'\x00')
#    cv2.imshow('display', img)
#    cv2.waitKey(1)

@sio.on('disconnect')
def disconnect(sid):
    print('>>> disconnect ', sid)

if __name__ == '__main__':
    eventlet.wsgi.server(eventlet.listen(('', 5000)), app)
