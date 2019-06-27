from websocket import create_connection
import numpy as np
import time
import collections
if __name__ == "__main__":

    ws = create_connection("ws://127.0.0.1:9090")
    print("Sending 'Hello, World'...")
    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()
    count = 9
    while True:
        if count%30 == 0:
            print("{} fps".format(1.0/np.mean(elapsed_time_queue)))
        data = np.zeros((640, 480, 4), dtype=np.uint8).tostring()
        #print(len(data))
        ws.send(data)
#        print("Receiving...")
#        result =  ws.recv()
#        print("Received '%s'" % result)

        end = time.time()
        elapsed = end - start
        start = time.time()
        elapsed_time_queue.append(elapsed)
        count += 1
        
    ws.close()
