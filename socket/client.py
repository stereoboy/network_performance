#
# references: https://www.geeksforgeeks.org/socket-programming-python/
#

# Import socket module
import socket

import numpy as np
import time
import collections
import argparse
import sys


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='This is toy client to test comm latency')
    parser.add_argument('-r', '--remote', action='store_true', default=False)
    parser.add_argument('-u', '--unix', action='store_true', default=False)
    options = parser.parse_args(sys.argv[1:])
    print(options)

    if options.remote:
        # Create a socket object
        s = socket.socket(socket.AF_UNIX)
        s.connect(('192.168.0.224', port))
    else:
        # Create a socket object
        s = socket.socket(socket.AF_INET)

        # Define the port on which you want to connect
        port = 9090

        if options.unix:
            s.connect('.unix_socket')
        else:
            # connect to the server on local computer
            s.connect(('127.0.0.1', port))

    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()
    global_start = time.time()
    count = 0
    err_cnt = 0

    while True:
        if count > 0 and count%100 == 0:
            print("{} fps".format(1.0/np.mean(elapsed_time_queue)))
        raw_data = np.zeros((640, 480, 4), dtype=np.uint8).reshape(-1, 1)
        raw_data[0] = 25
        offset = 0
        size   = 16384*10
        #size   = 640*480*4
        while True:
            #raw_data[offset] = 25
            data = raw_data[offset:offset + size].tostring()
            offset += size
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
            if offset >= len(raw_data):
                break
#        time.sleep(0.002)
        end = time.time()
        global_end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        count += 1

    # close the connection
    s.close()
