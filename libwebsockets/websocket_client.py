'''
    reference: https://github.com/websocket-client/websocket-client
'''
from websocket import create_connection
import numpy as np
import time
import collections
import cv2
import argparse
import sys

def main():

    parser = argparse.ArgumentParser(description='This is toy client to test comm latency')
    parser.add_argument('-r', '--remote', action='store_true', default=False)
    options = parser.parse_args(sys.argv[1:])
    print(options)

    img = cv2.imread('../image.png', cv2.IMREAD_UNCHANGED)
    print('img.shape={}'.format(img.shape))
    if options.remote:
        ws = create_connection("ws://192.168.0.224:9090")
    else:
        ws = create_connection("ws://127.0.0.1:9090")
    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()
    count = 0
    while True:
        data = img.tostring()
        ws.send_binary(data)
        #result =  ws.recv()

        end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        count += 1
        if count%100 == 0:
            print("{} fps".format(1.0/np.mean(elapsed_time_queue)))

    ws.close()

if __name__ == "__main__":
    main()
