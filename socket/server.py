#
# references: https://www.geeksforgeeks.org/socket-programming-python/
#

import time
import socket                
import collections
import numpy as np
import argparse
import sys

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='This is toy server to test comm latency')
    parser.add_argument('-u', '--unix', action='store_true', default=False)
    options = parser.parse_args(sys.argv[1:])
    print(options)

    if options.unix:
        # next create a socket object 
        s = socket.socket(socket.AF_UNIX)
        s.bind('.unix_socket')
        print("socket binded to .unix_socket")
    else:
        # next create a socket object 
        s = socket.socket()          
        print("Socket successfully created")

        # reserve a port on your computer in our 
        # case it is 12345 but it can be anything 
        port = 9090                

        # Next bind to the port 
        # we have not typed any ip in the ip field 
        # instead we have inputted an empty string 
        # this makes the server listen to requests  
        # coming from other computers on the network 
        s.bind(('', port))         
        print("socket binded to %s" %(port))

    # put the socket into listening mode 
    s.listen(5)      
    print("socket is listening")

    # a forever loop until we interrupt it or  
    # an error occurs 
    while True: 

        # Establish connection with client. 
        c, addr = s.accept()      
        print('Got connection from', addr)

        data_size = 640*480*4
        #data_size = 16384

        elapsed_time_queue = collections.deque(maxlen=100)
        start = time.time()
        global_start = time.time()
        count = 0
        closed = False

        buf = b""
        while True:
            if count > 0 and count%100 == 0:
                print("\t{} fps".format(1.0/np.mean(elapsed_time_queue)))

            # send a thank you message to the client.
            while True:
                recv_data = c.recv(data_size)
#            print("recv_data size:{}, recv_data[0]: {}".format(len(recv_data), recv_data[0]))
#            for i in range(len(recv_data)):
#                if recv_data[i] == 25:
#                    print(i)
                if len(buf) + len(recv_data) <= data_size:
                    remain_offset = len(recv_data)
                    buf += recv_data
                else:
                    remain_offset = data_size - len(buf)
                    buf += recv_data[:remain_offset]

#            print("\tbuf size: {}", len(buf))
                if len(buf) == data_size:
                    message = buf
                    buf = recv_data[remain_offset:]
                    break
                elif len(recv_data) == 0:
                    print("closed")
                    closed = True
                    break
                else:
                    #print("data size: {}<->{}".format(len(buf), data_size))
                    pass
                    #c.send("1")
                    #message = c.recv(640*48*4*4)

            end = time.time()
            global_end = time.time()
            elapsed = end - start
            start = time.time()
            elapsed_time_queue.append(elapsed)
            count += 1
            if closed:
                break
#        for i in range(data_size):
#            if message[i] == 25:
#                print(i)
#
#        print("message_size: {}, message_data: {}".format(len(message), message[0]))
            assert(message[0] == 25)

#        if count > 3:
#            break

        # Close the connection with the client 
        c.close()
