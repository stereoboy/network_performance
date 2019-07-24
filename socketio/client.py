import socketio
import numpy as np
import time
import collections

sio = socketio.Client()

@sio.on('connect')
def connect():
    print('connection established')

@sio.on('message')
def message(data):
    print('message received with ', data)
    sio.emit('my response', {'response': 'my response'})

@sio.on('disconnect')
def disconnect():
    print('disconnected from server')

if __name__ == '__main__':

    sio.connect('http://localhost:5000')
    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()
    count = 0

    while True:
        data = np.zeros((480, 640, 4), dtype=np.uint8).tostring()
        #print(len(data))
        sio.emit(data)
#        print("Receiving...")
#        result =  ws.recv()
#        print("Received '%s'" % result)

        end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        if count%30 == 0:
            print("{} fps".format(1.0/np.mean(elapsed_time_queue)))
        count += 1




