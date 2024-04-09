## References
* What is a Self-Signed Certificate? Advantages, Risks & Alternatives
  * https://www.keyfactor.com/blog/self-signed-certificate-risks/
* ssl — TLS/SSL wrapper for socket objects, SSLContext
  * https://docs.python.org/3/library/ssl.html#ssl-contexts

## Run
### Socket
```
python3 ./client_socket.py
```
```
python3 ./server_socket.py
```
```
$ sudo tcpdump -i lo  -vv -X   port 5000
tcpdump: listening on lo, link-type EN10MB (Ethernet), capture size 262144 bytes
21:41:41.602194 IP (tos 0x0, ttl 64, id 16567, offset 0, flags [DF], proto TCP (6), length 60)
    rofox-home-desktop.54410 > rofox-home-desktop.5000: Flags [S], cksum 0xf015 (incorrect -> 0xe4a0), seq 2245388923, win 65495, options [mss 65495,sackOK,TS val 2152964292 ecr 0,nop,wscale 7], length 0
        0x0000:  4500 003c 40b7 4000 4006 0a1e c0a8 374b  E..<@.@.@.....7K
        0x0010:  c0a8 374b d48a 1388 85d5 ea7b 0000 0000  ..7K.......{....
        0x0020:  a002 ffd7 f015 0000 0204 ffd7 0402 080a  ................
        0x0030:  8053 a0c4 0000 0000 0103 0307            .S..........
21:41:41.602204 IP (tos 0x0, ttl 64, id 0, offset 0, flags [DF], proto TCP (6), length 60)
    rofox-home-desktop.5000 > rofox-home-desktop.54410: Flags [S.], cksum 0xf015 (incorrect -> 0xbdb7), seq 177797939, ack 2245388924, win 65483, options [mss 65495,sackOK,TS val 2152964292 ecr 215296429
2,nop,wscale 7], length 0
        0x0000:  4500 003c 0000 4000 4006 4ad5 c0a8 374b  E..<..@.@.J...7K
        0x0010:  c0a8 374b 1388 d48a 0a98 fb33 85d5 ea7c  ..7K.......3...|
        0x0020:  a012 ffcb f015 0000 0204 ffd7 0402 080a  ................
        0x0030:  8053 a0c4 8053 a0c4 0103 0307            .S...S......
21:41:41.602212 IP (tos 0x0, ttl 64, id 16568, offset 0, flags [DF], proto TCP (6), length 52)
    rofox-home-desktop.54410 > rofox-home-desktop.5000: Flags [.], cksum 0xf00d (incorrect -> 0xe473), seq 1, ack 1, win 512, options [nop,nop,TS val 2152964292 ecr 2152964292], length 0
        0x0000:  4500 0034 40b8 4000 4006 0a25 c0a8 374b  E..4@.@.@..%..7K
        0x0010:  c0a8 374b d48a 1388 85d5 ea7c 0a98 fb34  ..7K.......|...4
        0x0020:  8010 0200 f00d 0000 0101 080a 8053 a0c4  .............S..
        0x0030:  8053 a0c4                                .S..
21:41:41.602325 IP (tos 0x0, ttl 64, id 16569, offset 0, flags [DF], proto TCP (6), length 569)
    rofox-home-desktop.54410 > rofox-home-desktop.5000: Flags [P.], cksum 0xf212 (incorrect -> 0x1db0), seq 1:518, ack 1, win 512, options [nop,nop,TS val 2152964292 ecr 2152964292], length 517
        0x0000:  4500 0239 40b9 4000 4006 081f c0a8 374b  E..9@.@.@.....7K
        0x0010:  c0a8 374b d48a 1388 85d5 ea7c 0a98 fb34  ..7K.......|...4
        0x0020:  8018 0200 f212 0000 0101 080a 8053 a0c4  .............S..
        0x0030:  8053 a0c4 1603 0102 0001 0001 fc03 03fb  .S..............
        0x0040:  8361 9de2 1f68 ccc2 c9e6 1c50 9d83 b9cd  .a...h.....P....
        0x0050:  c06d 1304 69ab 31ed 40e9 0ea4 c8c5 e020  .m..i.1.@.......
        0x0060:  7ff8 3c59 4927 41c3 4bda 3f64 45b9 ccab  ..<YI'A.K.?dE...
        0x0070:  f5bd 7929 d332 c0e7 6ab5 6709 2e80 e021  ..y).2..j.g....!
        0x0080:  003e 1302 1303 1301 c02c c030 009f cca9  .>.......,.0....
        0x0090:  cca8 ccaa c02b c02f 009e c024 c028 006b  .....+./...$.(.k
        0x00a0:  c023 c027 0067 c00a c014 0039 c009 c013  .#.'.g.....9....
        0x00b0:  0033 009d 009c 003d 003c 0035 002f 00ff  .3.....=.<.5./..
        0x00c0:  0100 0175 0000 0015 0013 0000 1063 6f6f  ...u.........coo
        0x00d0:  7072 6f62 6f74 6963 732e 636f 6d00 0b00  probotics.com...
        0x00e0:  0403 0001 0200 0a00 0c00 0a00 1d00 1700  ................
        0x00f0:  1e00 1900 1800 2300 0000 1600 0000 1700  ......#.........
        0x0100:  0000 0d00 2a00 2804 0305 0306 0308 0708  ....*.(.........
        0x0110:  0808 0908 0a08 0b08 0408 0508 0604 0105  ................
        0x0120:  0106 0103 0303 0103 0204 0205 0206 0200  ................
        0x0130:  2b00 0504 0304 0303 002d 0002 0101 0033  +........-.....3
        0x0140:  0026 0024 001d 0020 703d 5b5a 1529 7b30  .&.$....p=[Z.){0
        0x0150:  60e0 75d8 c1ce b660 e034 18bc af60 f3ce  `.u....`.4...`..
        0x0160:  cb2d 7bb8 c29c cd0c 0015 00cd 0000 0000  .-{.............

```
### No SSL Socket
```
python3 ./nossl_client_socket.py
```
```
python3 ./nossl_server_socket.py
```
```
$ sudo tcpdump -i lo  -vv -X   port 5000
[sudo] password for rofox:
tcpdump: listening on lo, link-type EN10MB (Ethernet), capture size 262144 bytes
22:21:05.966223 IP (tos 0x0, ttl 64, id 64046, offset 0, flags [DF], proto TCP (6), length 60)
    localhost.59184 > localhost.5000: Flags [S], cksum 0xfe30 (incorrect -> 0xc82f), seq 1560340435, win 65495, options [mss 65495,sackOK,TS val 2458367421 ecr 0,nop,wscale 7], length 0
        0x0000:  4500 003c fa2e 4000 4006 428b 7f00 0001  E..<..@.@.B.....
        0x0010:  7f00 0001 e730 1388 5d00 e7d3 0000 0000  .....0..].......
        0x0020:  a002 ffd7 fe30 0000 0204 ffd7 0402 080a  .....0..........
        0x0030:  9287 b5bd 0000 0000 0103 0307            ............
22:21:05.966233 IP (tos 0x0, ttl 64, id 0, offset 0, flags [DF], proto TCP (6), length 60)
    localhost.5000 > localhost.59184: Flags [S.], cksum 0xfe30 (incorrect -> 0x06c7), seq 149516341, ack 1560340436, win 65483, options [mss 65495,sackOK,TS val 2458367421 ecr 2458367421,nop,wscale 7], l
ength 0
        0x0000:  4500 003c 0000 4000 4006 3cba 7f00 0001  E..<..@.@.<.....
        0x0010:  7f00 0001 1388 e730 08e9 7035 5d00 e7d4  .......0..p5]...
        0x0020:  a012 ffcb fe30 0000 0204 ffd7 0402 080a  .....0..........
        0x0030:  9287 b5bd 9287 b5bd 0103 0307            ............
22:21:05.966241 IP (tos 0x0, ttl 64, id 64047, offset 0, flags [DF], proto TCP (6), length 52)
    localhost.59184 > localhost.5000: Flags [.], cksum 0xfe28 (incorrect -> 0x2d83), seq 1, ack 1, win 512, options [nop,nop,TS val 2458367421 ecr 2458367421], length 0
        0x0000:  4500 0034 fa2f 4000 4006 4292 7f00 0001  E..4./@.@.B.....
        0x0010:  7f00 0001 e730 1388 5d00 e7d4 08e9 7036  .....0..].....p6
        0x0020:  8010 0200 fe28 0000 0101 080a 9287 b5bd  .....(..........
        0x0030:  9287 b5bd                                ....
22:21:05.966259 IP (tos 0x0, ttl 64, id 64048, offset 0, flags [DF], proto TCP (6), length 84)
    localhost.59184 > localhost.5000: Flags [P.], cksum 0xfe48 (incorrect -> 0x0aac), seq 1:33, ack 1, win 512, options [nop,nop,TS val 2458367421 ecr 2458367421], length 32
        0x0000:  4500 0054 fa30 4000 4006 4271 7f00 0001  E..T.0@.@.Bq....
        0x0010:  7f00 0001 e730 1388 5d00 e7d4 08e9 7036  .....0..].....p6
        0x0020:  8018 0200 fe48 0000 0101 080a 9287 b5bd  .....H..........
        0x0030:  9287 b5bd 4845 4c4c 4f20 574f 524c 4420  ....HELLO.WORLD.
        0x0040:  3020 2020 2020 2020 2020 2020 2020 2020  0...............
        0x0050:  2020 2020                                ....
                           ...^

```
### Websockets
* https://websockets.readthedocs.io/en/stable/howto/quickstart.html
```
pip install websockets
python3 ./websocket_client.py
```
```
python3 ./websocket_socket.py
```

## Generation: CAbundle.pem, crtchain.pem, private.key
### References
* https://akshitb.medium.com/how-to-run-https-on-localhost-a-step-by-step-guide-c61fde893771
* https://wikidocs.net/125373
* https://www.ibm.com/docs/en/runbook-automation?topic=certificate-generate-root-ca-key
#### python socket with SSL
* https://pythontic.com/ssl/sslcontext/load_verify_locations
  > cafile – File path for file containing the CA Certtificate(s)
* https://pythontic.com/ssl/sslcontext/load_cert_chain
  > certfile            - Path of the X.509 certificate file in PEM(Privacy Enhanced Email) format.
  >
  > keyfile            - The private key of the certificate

### Issued By
* root.key or CA.key
```
$ openssl genrsa -out root.key 2048
Generating RSA private key, 2048 bit long modulus (2 primes)
.....................+++++
.......................................................................+++++
e is 65537 (0x010001)
```
* root.crt or CA.pem(CAbundle.pem) for Client from CA.key
```
$ openssl req -x509 -new -nodes -key root.key -sha256 -days 3650 -out root.crt
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:KR
State or Province Name (full name) [Some-State]:Seoul
Locality Name (eg, city) []:Seoul
Organization Name (eg, company) [Internet Widgits Pty Ltd]:CrazingLab
Organizational Unit Name (eg, section) []:R&D Lab
Common Name (e.g. server FQDN or YOUR name) []:jaeyoung lee
Email Address []:jy@wom.ai
```
### Issued To
#### server private key (server.key or private.key)
```
$ openssl genrsa -out server.key 2048
Generating RSA private key, 2048 bit long modulus (2 primes)
...................+++++
..................................................................+++++
e is 65537 (0x010001)
```
#### Cert Chain (certchain.pem or chain.pem)
* server.csr (certificate request) from server.key
```
$ openssl req -new -key server.key -out server.csr
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:US
State or Province Name (full name) [Some-State]:CA
Locality Name (eg, city) []:San Francisco
Organization Name (eg, company) [Internet Widgits Pty Ltd]:Coop Robotics
Organizational Unit Name (eg, section) []:Creative Labs
Common Name (e.g. server FQDN or YOUR name) []:cooprobotics.com
Email Address []:jy@cooprobotics.com

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:
An optional company name []:
```
* server.crt from server.csr and root.key
```
$ openssl x509 -req -in server.csr -CA root.crt -CAkey root.key -CAcreateserial -out server.crt -days 3650 -sha256
Signature ok
subject=C = US, ST = CA, L = San Francisco, O = Coop Robotics, OU = Creative Labs, CN = cooprobotics.com, emailAddress = jy@cooprobotics.com
Getting CA Private Key
```
* check server.crt
```
$ openssl x509 -in server.crt --text
Certificate:
    Data:
        Version: 1 (0x0)
        Serial Number:
            51:e0:53:27:13:7f:20:21:5b:a1:9c:11:9d:e6:d6:a3:22:50:38:fe
        Signature Algorithm: sha256WithRSAEncryption
        Issuer: C = KR, ST = Seoul, L = Seoul, O = CrazingLab, OU = R&D Lab, CN = jaeyoung lee, emailAddress = jy@wom.ai
        Validity
            Not Before: Mar 26 12:26:53 2024 GMT
            Not After : Mar 24 12:26:53 2034 GMT
        Subject: C = US, ST = CA, L = San Francisco, O = Coop Robotics, OU = Creative Labs, CN = cooprobotics.com, emailAddress = jy@cooprobotics.com
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                RSA Public-Key: (2048 bit)
                Modulus:
00:c8:d2:33:67:17:be:31:f9:f1:06:f8:e4:b4:25:
                    38:76:17:9a:b7:c0:f3:4c:9e:cf:a4:47:d6:56:d0:
                    ad:62:a2:26:d0:74:a7:3e:8e:c4:28:39:50:04:91:
                    97:fe:89:fc:37:9a:40:28:be:82:5e:f8:55:db:29:
                    17:7a:54:df:b8:4e:91:9b:67:7d:51:8c:d1:81:47:
                    8a:ce:bc:f3:a8:5c:f2:39:1b:96:db:85:a4:f5:3d:
                    ea:2e:2b:e0:a3:49:08:d8:0a:37:9f:a8:ca:1a:2e:
                    62:7b:c2:9e:56:bb:0f:54:33:e1:dd:f2:38:8b:be:
                    7e:dd:bf:90:9b:c0:aa:14:92:76:f3:1c:39:79:15:
                    87:ac:2d:ca:f8:dd:ac:88:6a:c2:31:09:f9:3e:44:
                    b7:b9:a5:8c:dc:25:d7:5c:8f:2e:3b:bd:3f:aa:7c:
                    41:02:52:76:3e:21:cd:54:4b:16:e0:b1:88:29:5f:
                    a0:f8:44:f2:52:7c:b9:d2:9b:d0:2b:4d:ae:70:b6:
                    b6:5d:1a:14:e3:3c:54:2b:5f:6f:b0:d5:29:f1:27:
                    1a:cf:e4:71:ee:b4:c3:6f:6f:1f:14:99:4c:7b:6c:
                    f3:b2:8a:e1:89:17:97:62:08:fb:9f:0b:6c:c3:bc:
                    1c:71:b6:9c:b1:d2:0c:7a:e4:b0:72:dc:1f:fb:87:
                    20:97
                Exponent: 65537 (0x10001)
    Signature Algorithm: sha256WithRSAEncryption
         1a:6e:c8:7c:d5:fe:69:71:dd:d5:a0:5b:f7:e7:b9:65:a6:e4:
         f9:f4:dc:0c:2c:dd:9e:f8:26:c2:61:bc:5a:fa:8e:0c:cd:33:
         17:71:dd:ad:03:8a:2e:93:99:4d:9b:be:a2:7f:f2:00:32:df:
         37:7a:73:76:8f:0a:ca:7d:b2:54:ae:d3:67:45:a1:3b:d1:62:
         ed:56:bb:2d:d1:92:df:ee:56:ba:79:51:07:19:d0:3f:b0:c8:
         4c:fd:ed:18:12:19:4a:8d:72:e1:13:a1:69:8b:33:93:05:bc:
         93:4d:49:dc:65:ff:a7:d3:1c:b6:02:94:27:a7:db:7e:01:5a:
         a7:87:20:1f:b6:41:9c:a7:16:f2:1e:6d:4e:f2:38:9a:4b:63:
         cd:54:75:1e:b7:88:a1:d8:82:ed:a4:87:86:16:7a:09:af:19:
         1a:6e:df:08:ed:2c:a4:e4:b4:2a:14:de:63:90:57:a2:fa:c8:
         f4:7f:f1:fd:2f:a5:dd:96:6b:3e:0e:c3:4a:b4:0d:c5:18:89:
         77:c0:bd:53:e6:cd:5f:64:5e:74:76:93:66:6d:3a:43:04:26:
         dc:a1:a2:b5:6e:74:9d:3d:eb:45:5a:05:87:d9:45:12:c3:b9:
         a7:4b:a4:9c:ab:1f:05:c7:98:93:1d:56:21:10:b3:2d:d6:d3:
         f1:c9:8d:44
-----BEGIN CERTIFICATE-----
MIIDrzCCApcCFFHgUycTfyAhW6GcEZ3m1qMiUDj+MA0GCSqGSIb3DQEBCwUAMIGF
MQswCQYDVQQGEwJLUjEOMAwGA1UECAwFU2VvdWwxDjAMBgNVBAcMBVNlb3VsMRMw
EQYDVQQKDApDcmF6aW5nTGFiMRAwDgYDVQQLDAdSJkQgTGFiMRUwEwYDVQQDDAxq
YWV5b3VuZyBsZWUxGDAWBgkqhkiG9w0BCQEWCWp5QHdvbS5haTAeFw0yNDAzMjYx
MjI2NTNaFw0zNDAzMjQxMjI2NTNaMIGhMQswCQYDVQQGEwJVUzELMAkGA1UECAwC
Q0ExFjAUBgNVBAcMDVNhbiBGcmFuY2lzY28xFjAUBgNVBAoMDUNvb3AgUm9ib3Rp
Y3MxFjAUBgNVBAsMDUNyZWF0aXZlIExhYnMxGTAXBgNVBAMMEGNvb3Byb2JvdGlj
cy5jb20xIjAgBgkqhkiG9w0BCQEWE2p5QGNvb3Byb2JvdGljcy5jb20wggEiMA0G
CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDI0jNnF74x+fEG+OS0JTh2F5q3wPNM
ns+kR9ZW0K1ioibQdKc+jsQoOVAEkZf+ifw3mkAovoJe+FXbKRd6VN+4TpGbZ31R
jNGBR4rOvPOoXPI5G5bbhaT1PeouK+CjSQjYCjefqMoaLmJ7wp5Wuw9UM+Hd8jiL
vn7dv5CbwKoUknbzHDl5FYesLcr43ayIasIxCfk+RLe5pYzcJddcjy47vT+qfEEC
UnY+Ic1USxbgsYgpX6D4RPJSfLnSm9ArTa5wtrZdGhTjPFQrX2+w1SnxJxrP5HHu
tMNvbx8UmUx7bPOyiuGJF5diCPufC2zDvBxxtpyx0gx65LBy3B/7hyCXAgMBAAEw
DQYJKoZIhvcNAQELBQADggEBABpuyHzV/mlx3dWgW/fnuWWm5Pn03Aws3Z74JsJh
vFr6jgzNMxdx3a0Dii6TmU2bvqJ/8gAy3zd6c3aPCsp9slSu02dFoTvRYu1Wuy3R
kt/uVrp5UQcZ0D+wyEz97RgSGUqNcuEToWmLM5MFvJNNSdxl/6fTHLYClCen234B
WqeHIB+2QZynFvIebU7yOJpLY81UdR63iKHYgu2kh4YWegmvGRpu3wjtLKTktCoU
3mOQV6L6yPR/8f0vpd2Waz4Ow0q0DcUYiXfAvVPmzV9kXnR2k2ZtOkMEJtyhorVu
dJ0960VaBYfZRRLDuadLpJyrHwXHmJMdViEQsy3W0/HJjUQ=
-----END CERTIFICATE-----

```