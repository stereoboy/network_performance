'''
    reference: https://stackoverflow.com/questions/54703656/python-socketio-how-to-emit-message-from-server-to-client
'''
import socketio
import numpy as np
import time
import collections
import argparse
import sys
# import cv2
import config
import logging

logger = logging.getLogger('Socketio.Client ')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter(fmt='%(asctime)s:[%(levelname)s][%(process)d][%(name)s] - %(funcName)s:%(lineno)d - %(message)s'))
logger.addHandler(handler)

sio = socketio.Client()

@sio.on('connect')
def connect():
    logger.info('connection established')

@sio.on('disconnect')
def disconnect():
    logger.info('disconnected from server')

def main():
    parser = argparse.ArgumentParser(description='This is toy client to test comm latency')
    parser.add_argument('-H', '--host', action='store', type=str, default='http://localhost')
    parser.add_argument('-p', '--port', action='store', type=int, default=config.PORT)
    parser.add_argument('-c', '--count', action='store', type=int, default=config.COUNT, help='measure frame rate after COUNT')

    options = parser.parse_args(sys.argv[1:])
    logger.info(options)

    #img = cv2.imread('../image.png', cv2.IMREAD_UNCHANGED)
    img = np.zeros((480, 640, 4), dtype=np.uint8)
    logger.info("sample image shape = {}".format(img.shape))

    try:
        sio.connect('{}:{}'.format(options.host, options.port), transports='polling')
        elapsed_time_queue = collections.deque(maxlen=options.count)
        start = time.time()
        count = 0

        while True:
            data = img.tobytes()
            logger.debug(len(data))
            if not sio.call('message', bytes(data)):
                logger.error("call failed")
                break
            end = time.time()
            elapsed = end - start
            start = time.time()
            elapsed_time_queue.append(elapsed)
            count += 1
            if count%options.count == 0:
                logger.info("\t{} fps".format(1.0/np.mean(elapsed_time_queue)))
    except Exception as e:
        logger.error("exit by {}: {}".format(type(e).__name__, e))
    except KeyboardInterrupt as e:
        logger.error("exit by KeyboardInterrupt")

if __name__ == '__main__':
    main()
