import socket

import argparse
import logging
import pathlib
import time
import sys

logger = logging.getLogger('STUN/UDP Client')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter(fmt='%(asctime)s:[%(levelname)s][%(process)d][%(name)s] - %(funcName)s:%(lineno)d - %(message)s'))
logger.addHandler(handler)

def main():
    parser = argparse.ArgumentParser(description='This is toy client to test STUN')
    parser.add_argument('-H', '--host', action='store', type=str, default='localhost')
    parser.add_argument('-p', '--port', action='store', type=int, default=8000)

    options = parser.parse_args(sys.argv[1:])
    logger.info("options={}".format(options))

    try:

        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as client_socket:
            while True:
                message = b"Hello, Server!"
                client_socket.sendto(message, (options.host, options.port))
                time.sleep(0.3)
    except KeyboardInterrupt:
        logger.info("exit by KeyboardInterrupt")
    except Exception as e:
        logger.error(f"exit by {type(e)}")

if __name__ == "__main__":
    main()