'''
    reference: https://stackoverflow.com/questions/54703656/python-socketio-how-to-emit-message-from-server-to-client
'''

import socketio
import numpy as np
import time
import collections
import queue
import argparse
import sys
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
    parser = argparse.ArgumentParser(description='This is toy client to test comm latency')
    parser.add_argument('-r', '--remote', action='store_true', default=False)
    options = parser.parse_args(sys.argv[1:])
    print(options)

    if options.remote:
        sio.connect('http://192.168.0.224:9090', transports='polling')
    else:
        #sio.connect('http://localhost:9090')
        sio.connect('http://127.0.0.1:9090', transports='polling')
    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()
    count = 0

    #img = cv2.imread('../image.png', cv2.IMREAD_UNCHANGED)
    img = np.zeros((480, 640, 4), dtype=np.uint8)
    print("sample image shape = {}".format(img.shape))

    while True:
        data = img.tostring()
        #print(len(data))
        sio.emit('ch1', bytes(data))
        q.get()
        end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        count += 1
        if count%100 == 0:
            print("{} fps".format(1.0/np.mean(elapsed_time_queue)))



