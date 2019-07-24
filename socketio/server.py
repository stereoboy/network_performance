import eventlet
import socketio
import numpy as np
import cv2
import time
import collections

sio = socketio.Server()
app = socketio.WSGIApp(sio, static_files={
    '/': {'content_type': 'text/html', 'filename': 'index.html'}
})

@sio.on('connect')
def connect(sid, environ):
    global count, elapsed_time_queue, start
    print('>>> connect ', sid)
    count = 0
    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()

@sio.on('ch1')
def on_message(sid, data):
    global count, elapsed_time_queue, start
    #print('>>> \tmessage')
    #img = np.frombuffer(data, dtype=np.uint8).reshape((480, 640, 4))
    end = time.time()
    elapsed = end - start
    start = time.time()
    elapsed_time_queue.append(elapsed)
    count += 1
    if count%100 == 0:
        print("{} fps".format(1.0/np.mean(elapsed_time_queue)))
    sio.emit('ch1', b'\x00')
#    cv2.imshow('display', img)
#    cv2.waitKey(1)

@sio.on('disconnect')
def disconnect(sid):
    print('>>> disconnect ', sid)

if __name__ == '__main__':
    eventlet.wsgi.server(eventlet.listen(('', 9090)), app)
