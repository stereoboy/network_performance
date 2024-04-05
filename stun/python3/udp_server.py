import socket

import argparse
import logging
import pathlib
import time
import sys

import stun

import netifaces

logger = logging.getLogger('STUN/UDP Server')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter(fmt='%(asctime)s:[%(levelname)s][%(process)d][%(name)s] - %(funcName)s:%(lineno)d - %(message)s'))
logger.addHandler(handler)

def main():
    #
    # references
    #  - https://stackoverflow.com/questions/30698521/python-netifaces-how-to-get-currently-used-network-interface
    #  - https://pypi.org/project/netifaces/
    #
    default_gateway_iface = netifaces.gateways()['default'][netifaces.AF_INET][1]
    logger.info(f"default_gateway_iface={default_gateway_iface}")

    for iface in netifaces.interfaces():
        ifaddresses = netifaces.ifaddresses(iface)
        ret = ifaddresses.get(netifaces.AF_INET, None)
        if ret and iface == default_gateway_iface:
            logger.info(f"default_hostname=[{iface}] {ret[0]['addr']}")
            default_hostname = ret[0]['addr']

    parser = argparse.ArgumentParser(description='This is toy server to test STUN')
    parser.add_argument('-H', '--host', action='store', type=str, default=default_hostname, help='type exact ip address')
    parser.add_argument('-p', '--port', action='store', type=int, default=8000)

    options = parser.parse_args(sys.argv[1:])
    logger.info("options={}".format(options))

    nat_type, external_ip, external_port = stun.get_ip_info(
                                                    stun_host="stun.l.google.com",
                                                    stun_port=19302,
                                                    source_ip=options.host,
                                                    source_port=options.port)

    logger.info(f"nat_type={nat_type}")
    logger.info(f"external_ip={external_ip}")
    logger.info(f"external_port={external_port}")

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as server_socket:
            server_socket.bind((options.host, options.port))
            logger.info("UDP Server started")
            while True:
                response, client = server_socket.recvfrom(1024)
                logger.info(f"Received response: {response} from: {client}")

    except KeyboardInterrupt:
        logger.info("exit by KeyboardInterrupt")
    # except Exception as e:
    #     logger.error(f"exit by {type(e)}")

if __name__ == "__main__":
    main()
