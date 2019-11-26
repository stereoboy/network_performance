#
# references: https://www.geeksforgeeks.org/socket-programming-python/
#

# Import socket module
import socket

import numpy as np
import time
import collections
import cv2


if __name__ == '__main__':

    # Create a socket object
    s = socket.socket()

    # Define the port on which you want to connect
    port = 9090

    # connect to the server on local computer
    #s.connect(('127.0.0.1', port))
    s.connect(('192.168.0.224', port))

    elapsed_time_queue = collections.deque(maxlen=100)
    time_queue0 = collections.deque(maxlen=10)
    time_queue1 = collections.deque(maxlen=10)
    start = time.time()
    global_start = time.time()
    count = 0
    err_cnt = 0

    #raw_data = np.zeros((640, 480, 4), dtype=np.uint8).flatten()
    bmp = cv2.imread('./image.png')

    _, jpg = cv2.imencode('.jpg', bmp, [cv2.IMWRITE_JPEG_QUALITY, 90])
    print("jpg size:{}".format(len(jpg)))

    while True:
        if count > 0 and count%100 == 0:
            print("{} fps".format(1.0/np.mean(elapsed_time_queue)))
        _, jpg = cv2.imencode('.jpg', bmp, [cv2.IMWRITE_JPEG_QUALITY, 90])
        offset = 0
        raw_data = jpg
#        header_data = np.array((len(jpg)), dtype=np.uint32).tostring()
#        s.send(header_data)
#        print(len(jpg))
#        raw_data = np.hstack([header, raw_data])
#        print("----------------------------------------")
        size   = 16384
        #size   = 640*480*4
#        header = np.array([], dtype=np.int32).tostring()
#        s.send(data)
        while True:
            #raw_data[offset] = 25
            data = raw_data[offset:offset + size].tostring()
            offset += size
            # receive data from the server
            b = time.time()
            sent = s.send(data)
            if sent != len(data):
                print("remain:{}".format(len(data) - sent))
            e = time.time()
            t = e - b
            time_queue0.append(t)
            if count > 0 and count%100 == 0:
                print("\t{} sec".format(np.mean(time_queue0)))
#        a = s.recv(10)
#        if a == "1":
#            err_cnt += 1
#            print("ERROR")
#            time.sleep(0.005)
#            data = np.zeros((640, 48, 4), dtype=np.uint8).tostring()
#            s.send(data)
            if offset >= len(raw_data):
                break
        time.sleep(0.004)
#        b = time.time()
#        r = s.recv(1)
#        e = time.time()
#        t = e - b
#        time_queue1.append(t)
#        if count > 0 and count%100 == 0:
#            print("\t\t{} sec".format(np.mean(time_queue1)))
        end = time.time()
        global_end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        count += 1

    # close the connection
    s.close()
