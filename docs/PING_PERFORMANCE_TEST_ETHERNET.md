## Test setup
<details open>
  <summary> show me </summary>

### Machine #1 (Dell Aurora R7 Desktop)
```
uname -a
Linux wom 5.15.0-94-generic #104-Ubuntu SMP Tue Jan 9 15:25:40 UTC 2024 x86_64 x86_64 x86_64 GNU/Linux
```
```
lsb_release -a
LSB Version:	core-11.1.0ubuntu4-noarch:security-11.1.0ubuntu4-noarch
Distributor ID:	Ubuntu
Description:	Ubuntu 22.04.2 LTS
Release:	22.04
Codename:	jammy
```
* `ifconfig enp6s0`
  ```
  enp6s0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
          inet 192.168.1.1  netmask 255.255.255.0  broadcast 192.168.1.255
          inet6 fe80::3356:710e:8b50:e566  prefixlen 64  scopeid 0x20<link>
          ether d8:9e:f3:98:c1:76  txqueuelen 1000  (Ethernet)
          RX packets 648  bytes 68020 (68.0 KB)
          RX errors 0  dropped 522  overruns 0  frame 0
          TX packets 128  bytes 21868 (21.8 KB)
          TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
          device interrupt 19

  ```
* `sudo lshw -class network`
  ```
    *-network
        description: Ethernet interface
        product: Killer E2500 Gigabit Ethernet Controller
        vendor: Qualcomm Atheros
        physical id: 0
        bus info: pci@0000:06:00.0
        logical name: enp6s0
        version: 10
        serial: d8:9e:f3:98:c1:76
        size: 1Gbit/s
        capacity: 1Gbit/s
        width: 64 bits
        clock: 33MHz
        capabilities: pm pciexpress msi msix bus_master cap_list ethernet physical tp 10bt 10bt-fd 100bt 100bt-fd 1000bt-fd autonegotiation
        configuration: autonegotiation=on broadcast=yes driver=alx driverversion=5.15.0-94-generic duplex=full ip=192.168.1.1 latency=0 link=yes multicast=yes port=twisted pair speed=1Gbit/s
        resources: irq:19 memory:ec400000-ec43ffff ioport:d000(size=128)

  ```

### Machine #2 (Orin)
```
uname -a
Linux nesfr7-01 5.10.104-tegra #1 SMP PREEMPT Wed Aug 10 20:17:07 PDT 2022 aarch64 aarch64 aarch64 GNU/Linux
```
```
lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 20.04.4 LTS
Release:        20.04
Codename:       focal
```
* `ifconfig`
  ```
  eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1466
          inet 192.168.1.2  netmask 255.255.255.0  broadcast 192.168.1.255
          inet6 fe80::856:5412:f110:5eb5  prefixlen 64  scopeid 0x20<link>
          ether 48:b0:2d:a4:f7:f0  txqueuelen 1000  (Ethernet)
          RX packets 126  bytes 14689 (14.6 KB)
          RX errors 0  dropped 87  overruns 0  frame 0
          TX packets 46  bytes 7034 (7.0 KB)
          TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
  ```
* `lshw -class network`
  ```
    *-network:1
        description: Ethernet interface
        physical id: 2
        logical name: eth0
        serial: 48:b0:2d:a4:f7:f0
        size: 1Gbit/s
        capacity: 10Gbit/s
        capabilities: ethernet physical 100bt 100bt-fd 1000bt-fd 10000bt-fd autonegotiation
        configuration: autonegotiation=on broadcast=yes driver=nvethernet driverversion=5.10.104-tegra duplex=full ip=192.168.1.2 link=yes multicast=yes port=twisted pair speed=1Gbit/s

  ```
</details>

## Direct Ethernet Cable Wired without Switch
### Results (2024-02-16)
```
--- 192.168.2.3 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99385ms
rtt min/avg/max/mdev = 0.364/0.928/1.925/0.136 ms
```
```
--- 192.168.2.3 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99230ms
rtt min/avg/max/mdev = 0.563/1.005/1.270/0.102 ms
```
```
--- 192.168.2.3 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99163ms
rtt min/avg/max/mdev = 0.350/1.019/1.139/0.096 ms
```

## Ethernet Cable Wired with Switch
### Switch
* tp-link `TL-SG105E` (https://www.tp-link.com/us/business-networking/easy-smart-switch/tl-sg105e/v1/)
```
* 5-Port Gigabit Easy Smart Switch
* 5 10/100/1000Mbps RJ45 ports
* Plug and play, with no configuration required
* Durable metal casing of superior quality and professional appearance
* Intelligent management via a web user interface and downloadable utility
* Green technology reduces power consumption
```
### Results (2024-02-16)
```
--- 192.168.2.3 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99276ms
rtt min/avg/max/mdev = 0.336/0.996/1.641/0.119 ms
```
```
--- 192.168.2.3 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99186ms
rtt min/avg/max/mdev = 0.683/1.015/1.253/0.108 ms
```
```
--- 192.168.2.3 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99259ms
rtt min/avg/max/mdev = 0.355/0.989/1.227/0.116 ms
```
## Ethernet Cable with Routers
### Router #1
* ipTIME `AX5400BCM-6E`
  * https://iptime.com/iptime/?page_id=11&pf=2&page=&pt=683&pd=1
  * https://namu.wiki/w/ipTIME%20AX5400BCM-6E
```
프로세서 Broadcom BCM6756 Quad-Core CPU (1.7GHz)
메모리 DDR3 512MB
저장소 128 MB NAND Flash
Wi-Fi 규격 Wi-Fi 1/2/3/4/5/6/6E
단자정보 RJ-45 1Gb WAN x 1 / RJ-45 1Gb LAN x 4
안테나 6GHz x 2 (2Tx-2Rx) 5GHz/2.4GHz x 2 (2Tx-2Rx)
크기 110 x 110 x 229 mm (안테나 크기 제외)
기타 Tri-band 및 빔포밍(Beamforming) 지원
```
#### Results (2024-02-16)
```
--- 192.168.2.3 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99229ms
rtt min/avg/max/mdev = 0.337/0.996/1.855/0.147 ms
```
```
--- 192.168.2.3 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99375ms
rtt min/avg/max/mdev = 0.442/0.994/1.474/0.123 ms
```
```
--- 192.168.2.3 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99224ms
rtt min/avg/max/mdev = 0.348/0.987/1.253/0.105 ms

```