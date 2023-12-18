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
import config
import logging

logger = logging.getLogger('Socketio.Client ')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter(fmt='%(asctime)s:[%(levelname)s][%(process)d][%(name)s] - %(funcName)s:%(lineno)d - %(message)s'))
logger.addHandler(handler)

q = queue.Queue()
sio = socketio.Client()

@sio.on('connect')
def connect():
    logger.info('connection established')

@sio.on('ch1')
def message(data):
    logger.debug('>>> message received with ', data)
    q.put(data)

@sio.on('disconnect')
def disconnect():
    logger.info('disconnected from server')

def main():
    parser = argparse.ArgumentParser(description='This is toy client to test comm latency')
    parser.add_argument('-H', '--host', action='store', type=str, default='http://localhost')
    parser.add_argument('-p', '--port', action='store', type=int, default=config.PORT)

    options = parser.parse_args(sys.argv[1:])
    logger.info(options)


    sio.connect('{}:{}'.format(options.host, options.port), transports='polling')
    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()
    count = 0

    #img = cv2.imread('../image.png', cv2.IMREAD_UNCHANGED)
    img = np.zeros((480, 640, 4), dtype=np.uint8)
    logger.info("sample image shape = {}".format(img.shape))

    while True:
        data = img.tobytes()
        logger.debug(len(data))
        sio.emit('ch1', bytes(data))
        q.get()
        end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        count += 1
        if count%100 == 0:
            logger.info("\t{} fps".format(1.0/np.mean(elapsed_time_queue)))

if __name__ == '__main__':
    main()