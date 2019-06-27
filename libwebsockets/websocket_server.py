import struct

from socket import error

from tornado.ioloop import IOLoop
from tornado.websocket import WebSocketHandler

from tornado.ioloop import IOLoop
from tornado.ioloop import PeriodicCallback
from tornado.web import Application

import tornado.options
from tornado.log import access_log, gen_log, app_log

import traceback
import logging
import sys
import numpy as np
import cv2

class RosbridgeWebSocket(WebSocketHandler):
  client_id_seed = 0
  clients_connected = 0
  authenticate = False

  # The following are passed on to RosbridgeProtocol
  # defragmentation.py:
  fragment_timeout = 600                  # seconds
  # protocol.py:
  delay_between_messages = 0              # seconds
  max_message_size = None                 # bytes
  unregister_timeout = 10.0               # seconds
  bson_only_mode = False

  def __init__(self, application, request, **kwargs):
    super(RosbridgeWebSocket, self).__init__(application, request, **kwargs)

  def open(self):
    print("open()")

  def on_message(self, message):
    print("message_size: {}".format(len(message)))

  def on_close(self):
    print("on_close()")

  def send_message(self, message):
    print("send_message()")

  def get_compression_options(self):
    return None


if __name__ == "__main__":

  tornado.options.parse_command_line()

  application = Application([(r"/", RosbridgeWebSocket), (r"", RosbridgeWebSocket)])

  connected = False
  while not connected:
    try:
      application.listen(9090, "")
      connected = True
    except error as e:
      print(e)

  print("RosFastImgMsgServer Initialized!")
  IOLoop.instance().start()
