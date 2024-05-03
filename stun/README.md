## python3

### Punching Hole
#### References
* https://github.com/engineer-man/youtube/blob/master/141/client.py
* https://www.youtube.com/watch?v=TiMeoQt3K4g

    [<img src="https://img.youtube.com/vi/TiMeoQt3K4g/0.jpg" alt="drawing" width="240"/>](https://youtu.be/TiMeoQt3K4g)

* https://www.youtube.com/watch?v=IbzGL_tjmv4

    [<img src="https://img.youtube.com/vi/IbzGL_tjmv4/0.jpg" alt="drawing" width="240"/>](https://youtu.be/IbzGL_tjmv4)

* https://doc-kurento.readthedocs.io/en/latest/knowledge/nat.html

#### Run
* Host A
```
python3 ./udp_signaling_server.py
2024-05-03 10:18:41,576:[INFO] - options=Namespace(port=5000)
2024-05-03 10:18:41,577:[INFO] - Server listening on port 5000
2024-05-03 10:18:49,397:[INFO] - connection from: ('54.193.231.0', 5001)
2024-05-03 10:18:56,377:[INFO] - connection from: ('221.150.237.133', 5003)
2024-05-03 10:18:56,377:[INFO] - got 2 clients, sending details to each
2024-05-03 10:18:56,377:[INFO] - Server listening on port 5000

```
* Host B
```
python3 ./udp_peer.py -H 54.193.231.0 -d 5003
2024-05-03 19:18:56,285:[INFO][STUN/UDP Peer] - options=Namespace(host='54.193.231.0', port=5000, data_port=5003)
2024-05-03 19:18:56,285:[INFO][STUN/UDP Peer] - connecting to signaling server
2024-05-03 19:18:56,428:[INFO][STUN/UDP Peer] - checked in with server, waiting
2024-05-03 19:18:56,428:[INFO][STUN/UDP Peer] -
got peer
2024-05-03 19:18:56,428:[INFO][STUN/UDP Peer] -   ip:          54.193.231.0
2024-05-03 19:18:56,428:[INFO][STUN/UDP Peer] -   source port: 5001
2024-05-03 19:18:56,428:[INFO][STUN/UDP Peer] - punching hole
2024-05-03 19:18:56,428:[INFO][STUN/UDP Peer] - ready to exchange messages

> hey from B
peer: hey from C
>
```
* Host C
```
python3 ./udp_peer.py -H 54.193.231.0 -d 5001
2024-05-03 10:18:49,395:[INFO][STUN/UDP Peer] - options=Namespace(data_port=5001, host='54.193.231.0', port=5000)
2024-05-03 10:18:49,396:[INFO][STUN/UDP Peer] - connecting to signaling server
2024-05-03 10:18:49,397:[INFO][STUN/UDP Peer] - checked in with server, waiting
2024-05-03 10:18:56,378:[INFO][STUN/UDP Peer] -
got peer
2024-05-03 10:18:56,378:[INFO][STUN/UDP Peer] -   ip:          221.150.237.133
2024-05-03 10:18:56,379:[INFO][STUN/UDP Peer] -   source port: 5003
2024-05-03 10:18:56,379:[INFO][STUN/UDP Peer] - punching hole
2024-05-03 10:18:56,379:[INFO][STUN/UDP Peer] - ready to exchange messages

peer: 0
peer: hey from B
> hey from C
>
```
### pystun3
* https://pypi.org/project/pystun3/
* https://github.com/talkiq/pystun3
```
pip install pystun3
```

* cli
```
$ pystun3 -H stun.l.google.com -P 19302
NAT Type: Full Cone
External IP: 211.210.70.216
External Port: 54320
Press any key to continue
```