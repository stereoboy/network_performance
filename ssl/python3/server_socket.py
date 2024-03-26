import socket
import ssl

import pathlib
import logging
#
# refereces
#  - https://docs.python.org/3/library/ssl.html#socket-creation
#  - https://wikidocs.net/125373
#
logger = logging.getLogger('TCP/SSLServer')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter(fmt='%(asctime)s:[%(levelname)s][%(name)s] - %(funcName)s:%(lineno)d - %(message)s'))
logger.addHandler(handler)

def main():

    context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    certfile  = pathlib.Path(__file__).with_name('server.crt')
    keyfile   = pathlib.Path(__file__).with_name('server.key')
    context.load_cert_chain(certfile=certfile, keyfile=keyfile)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as sock:
        sock.bind(('127.0.0.1', 5000))
        sock.listen()
        with context.wrap_socket(sock, server_side=True) as ssock:
            conn, addr = ssock.accept()

            try:
                count = 0
                while True:
                    data = conn.recv(32).decode('utf8')
                    if len(data) == 0:
                        logger.info('Connection closed')
                        break
                    logger.info(f'{data}')
                    data = bytes('{:<32}'.format("hello world {}".format(count)), 'utf-8')
                    conn.sendall(data)
                    count += 1
            except KeyboardInterrupt as e:
                logger.info("exit by {}".format(type(e)))
            except ConnectionResetError as e:
                logger.info("exit by {}".format(type(e)))
            except:
                logger.error("exit by {}".format(type(e)))

if __name__ == '__main__':
    main()