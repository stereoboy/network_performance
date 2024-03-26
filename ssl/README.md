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