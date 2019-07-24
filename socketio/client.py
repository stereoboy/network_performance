'''
    reference: https://stackoverflow.com/questions/54703656/python-socketio-how-to-emit-message-from-server-to-client
'''

import socketio
import numpy as np
import time
import collections
import queue
import cv2

q = queue.Queue()
sio = socketio.Client()

@sio.on('connect')
def connect():
    print('connection established')

@sio.on('ch1')
def message(data):
    #print('>>> message received with ', data)
    q.put(data)

@sio.on('disconnect')
def disconnect():
    print('disconnected from server')

if __name__ == '__main__':

    sio.connect('http://localhost:5000')
    #sio.connect('http://192.168.0.224:5000')
    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()
    count = 0

    img = cv2.imread('../image.png', cv2.IMREAD_UNCHANGED)

    while True:
        data = img.tostring()
        #print(len(data))
        sio.emit('ch1', data)
        q.get()
        end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        if count%30 == 0:
            print("{} fps".format(1.0/np.mean(elapsed_time_queue)))
        count += 1




