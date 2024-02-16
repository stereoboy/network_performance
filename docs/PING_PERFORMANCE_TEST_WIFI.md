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
* `ifconfig wlp5s0`
  ```
  wlp5s0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
          inet 192.168.0.2  netmask 255.255.255.0  broadcast 192.168.0.255
          inet6 fe80::cdee:a49f:f2b0:bdf6  prefixlen 64  scopeid 0x20<link>
          ether 9c:b6:d0:b7:d5:b5  txqueuelen 1000  (Ethernet)
          RX packets 84679  bytes 113313375 (113.3 MB)
          RX errors 0  dropped 5  overruns 0  frame 0
          TX packets 28811  bytes 5548014 (5.5 MB)
          TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

  ```
* `sudo lshw -class network`
  ```
  *-network
      description: Wireless interface
      product: QCA6174 802.11ac Wireless Network Adapter
      vendor: Qualcomm Atheros
      physical id: 0
      bus info: pci@0000:05:00.0
      logical name: wlp5s0
      version: 32
      serial: 9c:b6:d0:b7:d5:b5
      width: 64 bits
      clock: 33MHz
      capabilities: pm msi pciexpress bus_master cap_list ethernet physical wireless
      configuration: broadcast=yes driver=ath10k_pci driverversion=5.15.0-94-generic firmware=WLAN.RM.4.4.1-00157-QCARMSWPZ-1 ip=192.168.0.2 latency=0 link=yes multicast=yes wireless=IEEE 802.11
      resources: irq:145 memory:ec200000-ec3fffff

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
#### WIFI #1
* `ifconfig wlan1`
  ```
  wlan1: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
          inet 192.168.0.13  netmask 255.255.255.0  broadcast 192.168.0.255
          inet6 fe80::a25d:7e97:58a0:fc95  prefixlen 64  scopeid 0x20<link>
          ether 58:86:94:f3:1c:0c  txqueuelen 1000  (Ethernet)
          RX packets 324251  bytes 461167308 (461.1 MB)
          RX errors 0  dropped 3123  overruns 0  frame 0
          TX packets 118502  bytes 34284812 (34.2 MB)
          TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

  ```
* `lshw -class network`
  ```
  *-network:4
      description: Wireless interface
      physical id: 7
      bus info: usb@1:4.1
      logical name: wlan1
      serial: 58:86:94:f3:1c:0c
      capabilities: ethernet physical wireless
      configuration: broadcast=yes driver=8814au driverversion=5.10.104-tegra ip=192.168.0.13 multicast=yes wireless=IEEE 802.11

  ```

* `iwconfig wlan1`
  ```
  wlan1     IEEE 802.11  ESSID:"NESFRVR_5G"
            Mode:Managed  Frequency:5.745 GHz  Access Point: 70:5D:CC:D4:38:9B
            Bit Rate=1.3 Gb/s   Tx-Power=20 dBm
            Retry short limit:7   RTS thr:off   Fragment thr:off
            Power Management:on
            Link Quality=64/70  Signal level=-46 dBm
            Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0
            Tx excessive retries:0  Invalid misc:0   Missed beacon:0

  ```
#### WIFI #2
* `ifconfig wlan0`
```
wlan0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.0.18  netmask 255.255.255.0  broadcast 192.168.0.255
        inet6 fe80::f2a1:5ead:aaad:f0de  prefixlen 64  scopeid 0x20<link>
        ether 90:e8:68:bc:70:91  txqueuelen 1000  (Ethernet)
        RX packets 18  bytes 3169 (3.1 KB)
        RX errors 0  dropped 2  overruns 0  frame 0
        TX packets 44  bytes 8054 (8.0 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

```
* `lshw -class network`
```
*-network
    description: Wireless interface
    product: RTL8822CE 802.11ac PCIe Wireless Network Adapter
    vendor: Realtek Semiconductor Co., Ltd.
    physical id: 0
    bus info: pci@0001:01:00.0
    logical name: wlan0
    version: 00
    serial: 90:e8:68:bc:70:91
    width: 64 bits
    clock: 33MHz
    capabilities: pm msi pciexpress bus_master cap_list ethernet physical wireless
    configuration: broadcast=yes driver=rtl88x2ce driverversion=5.10.104-tegra ip=192.168.0.18 latency=0 multicast=yes wireless=IEEE 802.11AC
    resources: irq:310 ioport:1000(size=256) memory:20a8000000-20a800ffff
```
* `iwconfig wlan0`
  ```
  wlan0     IEEE 802.11AC  ESSID:"NESFRVR_5G"  Nickname:"<WIFI@REALTEK>"
            Mode:Managed  Frequency:5.745 GHz  Access Point: 70:5D:CC:D4:38:9B
            Bit Rate:867 Mb/s   Sensitivity:0/0
            Retry:off   RTS thr:off   Fragment thr:off
            Power Management:off
            Link Quality=100/100  Signal level=53/100  Noise level=0/100
            Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0
            Tx excessive retries:0  Invalid misc:0   Missed beacon:0

  ```

</details>

## Ethernet Cable with Routers
### Router #2

### Router #2 - WIFI #1


#### Results (2024-02-16)
```
--- 192.168.0.13 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99156ms
rtt min/avg/max/mdev = 3.983/9.927/89.077/12.831 ms
```
```
--- 192.168.0.13 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99160ms
rtt min/avg/max/mdev = 4.344/12.071/34.972/6.102 ms
```
```
--- 192.168.0.13 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99152ms
rtt min/avg/max/mdev = 3.967/17.752/172.607/22.183 ms

```
```
--- 192.168.0.13 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99169ms
rtt min/avg/max/mdev = 4.116/17.046/181.582/23.268 ms
```
```
--- 192.168.0.13 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99159ms
rtt min/avg/max/mdev = 4.299/11.557/39.139/6.949 ms

```
```
--- 192.168.0.13 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99153ms
rtt min/avg/max/mdev = 3.781/15.420/162.962/19.115 ms
```

### Router #2 - WIFI #2


#### Results (2024-02-16)
```
--- 192.168.0.18 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99165ms
rtt min/avg/max/mdev = 3.161/14.756/51.683/8.774 ms
```
```
--- 192.168.0.18 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99160ms
rtt min/avg/max/mdev = 4.048/12.079/45.516/8.200 ms
```
```
--- 192.168.0.18 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99160ms
rtt min/avg/max/mdev = 2.706/17.065/52.805/10.899 ms
```
```
--- 192.168.0.18 ping statistics ---
100 packets transmitted, 100 received, 0% packet loss, time 99154ms
rtt min/avg/max/mdev = 3.355/16.606/68.783/12.920 ms
```