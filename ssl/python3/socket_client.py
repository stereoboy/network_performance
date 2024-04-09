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
    parser = argparse.ArgumentParser(description='TCP/SSL Client')
    parser.add_argument('-s', '--server-hostname', action='store', dest='hostname', default='localhost', help='server hostname')
    parser.add_argument('-V', '--verify', action='store_true', dest='verify', default=False, help='verify server certificate')
    options = parser.parse_args(sys.argv[1:])

    if not options.verify:
        #
        # reference: https://docs.python.org/3/library/ssl.html#ssl.SSLContext
        #
        logger.info("SSLContext without certificate verification")
        context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
        context.check_hostname = False
        context.verify_mode = ssl.CERT_NONE
    else:
        cafile = pathlib.Path(__file__).with_name("root.crt")
        logger.info(f"SSLContext with certificate verification: {cafile}")

        # PROTOCOL_TLS_CLIENT requires valid cert chain and hostname
        context = ssl.create_default_context()
        context.load_verify_locations(cafile)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as sock:
        with context.wrap_socket(sock, server_hostname="cooprobotics.com") as ssock:
            logger.info('Trying to connect to {}:5000'.format(options.hostname))
            ssock.connect((options.hostname, 5000))

            logger.info('SSL Version: {}'.format(ssock.version()))
            try:
                count = 0
                while True:
                    data = bytes('{:<32}'.format("HELLO WORLD {}".format(count)), 'utf-8')
                    ssock.sendall(data)
                    data = ssock.recv(32).decode('utf-8')
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