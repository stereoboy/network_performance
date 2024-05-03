import socket
import sys

import argparse
import logging

logger = logging.getLogger('STUN/UDP Signaling Server')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter(fmt='%(asctime)s:[%(levelname)s] - %(message)s'))
logger.addHandler(handler)

def main():
    parser = argparse.ArgumentParser(description='This is toy signaling server to test STUN')
    parser.add_argument('-p', '--port', action='store', type=int, default=5000, help='type signaling port')
    options = parser.parse_args(sys.argv[1:])
    logger.info("options={}".format(options))

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #sock.bind(('172.31.13.19', 5000))
    sock.bind(('0.0.0.0', 5000))

    while True:
        clients = []

        logger.info("Server listening on port {}".format(options.port))
        while True:
            data, address = sock.recvfrom(128)

            logger.info('data={}'.format(data))

            ip, port = data.decode().split(' ')
            address = (ip, int(port))
            logger.info('connection from: {}'.format(address))
            clients.append(address)

            sock.sendto(b'ready', address)

            if len(clients) == 2:
                logger.info('got 2 clients, sending details to each')
                break

        c1 = clients.pop()
        c1_addr, c1_port = c1
        c2 = clients.pop()
        c2_addr, c2_port = c2

        sock.sendto('{} {}'.format(c1_addr, c1_port).encode(), c2)
        sock.sendto('{} {}'.format(c2_addr, c2_port).encode(), c1)


if __name__ == "__main__":
    main()