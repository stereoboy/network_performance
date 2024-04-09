import socket
import ssl

import sys
import logging
import pathlib
import time
import argparse

#
# refereces
#  - https://docs.python.org/3/library/ssl.html#socket-creation
#  - https://wikidocs.net/125373
#
logger = logging.getLogger('TCP/SSL Client')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter(fmt='%(asctime)s:[%(levelname)s][%(name)s] - %(funcName)s:%(lineno)d - %(message)s'))
logger.addHandler(handler)

def main():
    parser = argparse.ArgumentParser(description='TCP/Client')
    parser.add_argument('-s', '--server-hostname', action='store', dest='hostname', default='localhost', help='server hostname')
    options = parser.parse_args(sys.argv[1:])


    with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as sock:

        logger.info('Trying to connect to {}:5000'.format(options.hostname))
        sock.connect((options.hostname, 5000))

        try:
            count = 0
            while True:
                data = bytes('{:<32}'.format("HELLO WORLD {}".format(count)), 'utf-8')
                sock.sendall(data)
                data = sock.recv(32).decode('utf-8')
                if len(data) == 0:
                    logger.info('Connection closed')
                    break
                logger.info(f"{data}")
                count += 1
                time.sleep(0.1)
        except KeyboardInterrupt as e:
            logger.info("exit by {}".format(type(e)))

if __name__ == '__main__':
    main()