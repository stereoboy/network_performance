#
# references: https://www.geeksforgeeks.org/socket-programming-python/
#

# Import socket module
import socket

import numpy as np
import time
import collections


if __name__ == '__main__':

    # Create a socket object
    s = socket.socket()

    # Define the port on which you want to connect
    port = 9090

    # connect to the server on local computer
    s.connect(('127.0.0.1', port))
    #s.connect(('192.168.0.224', port))

    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()
    global_start = time.time()
    count = 0
    err_cnt = 0

    while True:
        if count > 0 and count%100 == 0:
            print("{} fps".format(1.0/np.mean(elapsed_time_queue)))
            print("\terror {} fps".format(err_cnt/(global_end - global_start)))
        data = np.zeros((640, 480, 4), dtype=np.uint8).tostring()
        # receive data from the server
        sent = s.send(data)
        if sent != len(data):
            print("remain:{}".format(len(data) - sent))
#        a = s.recv(10)
#        if a == "1":
#            err_cnt += 1
#            print("ERROR")
#            time.sleep(0.005)
#            data = np.zeros((640, 48, 4), dtype=np.uint8).tostring()
#            s.send(data)
        time.sleep(0.004)
        end = time.time()
        global_end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        count += 1

    # close the connection
    s.close()
