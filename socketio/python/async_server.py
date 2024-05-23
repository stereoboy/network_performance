from aiohttp import web
import socketio
import numpy as np
import time
import collections
import config
import logging

logger = logging.getLogger('Socketio.Server')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setFormatter(logging.Formatter(fmt='%(asctime)s:[%(levelname)s][%(process)d][%(name)s] - %(funcName)s:%(lineno)d - %(message)s'))
logger.addHandler(handler)
# logging.basicConfig(level=logging.WARNING, format='%(asctime)s:[%(levelname)s][%(process)d][%(name)s] - %(funcName)s:%(lineno)d - %(message)s')


# sio = socketio.Server(logger=logger, engineio_logger=True, max_http_buffer_size=480*640*4*2)
sio = socketio.AsyncServer(logger=False, engineio_logger=False, max_http_buffer_size=480*640*4*2)
#app = socketio.WSGIApp(sio, static_files={
#    '/': {'content_type': 'text/html', 'filename': 'index.html'}
#})
app = web.Application()
sio.attach(app)

@sio.on('connect')
def connect(sid, environ):
    global count, elapsed_time_queue, start
    logger.info('>>> connect {}'.format(sid))
    for k, v in environ.items():
        logger.info('\t{}:{}'.format(k, v))

    count = 0
    elapsed_time_queue = collections.deque(maxlen=100)
    start = time.time()

@sio.on('message')
async def on_message(sid, data):
    global count, elapsed_time_queue, start
    logger.debug('>>> \tmessage: len(data)={}'.format(len(data)))
    #img = np.frombuffer(data, dtype=np.uint8).reshape((480, 640, 4))
    logger.info(data.decode())
    end = time.time()
    elapsed = end - start
    start = time.time()
    elapsed_time_queue.append(elapsed)
    count += 1
    if count%100 == 0:
        logger.info("\t{} fps".format(1.0/np.mean(elapsed_time_queue)))
    return True

@sio.on('disconnect')
def disconnect(sid):
    logger.info('>>> disconnect ', sid)

async def index(request):
    """Serve the client-side application."""
    with open('./static/index.html') as f:
        return web.Response(text=f.read(), content_type='text/html')

if __name__ == '__main__':
    logger.info('python-socket.io async server')
#    eventlet.wsgi.server(eventlet.listen(('', config.PORT)), app)
    raise NotImplementedError
    app.router.add_static('/static', 'static')
    app.router.add_get('/', index)
    web.run_app(app, host='0.0.0.0', port=config.PORT)
