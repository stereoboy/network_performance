import socket
import ssl

import logging
import pathlib
import time

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
    # PROTOCOL_TLS_CLIENT requires valid cert chain and hostname
    context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
    cafile = pathlib.Path(__file__).with_name("root.crt")

    context.load_verify_locations(cafile)

    # with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as sock:
    #     sock.connect(('127.0.0.1', 5000))

    #     count = 0
    #     while True:
    #         data = bytes('{:<32} {}'.format("HELLO WORLD", count), 'utf-8')
    #         sock.sendall(data)
    #         data = sock.recv(32).decode('utf-8')
    #         logger.info(f"{data}")
    #         count += 1
    #         time.sleep(0.1)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as sock:
        with context.wrap_socket(sock, server_hostname="cooprobotics.com") as ssock:
            logger.info(ssock.version())
            ssock.connect(('127.0.0.1', 5000))

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
            except:
                logger.error("exit by {}".format(type(e)))

if __name__ == '__main__':
    main()