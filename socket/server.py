#
# references: https://www.geeksforgeeks.org/socket-programming-python/
#

import time
import socket                
import collections
import numpy as np

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

    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()
    global_start = time.time()
    count = 0
    closed = False

    while True:
        if count > 0 and count%100 == 0:
            print("\t{} fps".format(1.0/np.mean(elapsed_time_queue)))

        # send a thank you message to the client.
        buf = b""
        while True:
            recv_data = c.recv(data_size)
            if len(buf) + len(recv_data) <= data_size:
                remain_offset = len(recv_data)
                buf += recv_data
            else:
                remain_offset = data_size - len(buf)
                buf += recv_data[:remain_offset]

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
        print("message_size: {}".format(len(message)))
        end = time.time()
        global_end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        count += 1
        if closed:
            break

    # Close the connection with the client 
    c.close()
