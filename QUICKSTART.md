# Quick Start

## Prerequisites

- Linux system (x86_64)
- Kernel headers installed: `sudo apt install linux-headers-$(uname -r)`
- Root access for loading kernel modules

## Build and Run

```
make build
sudo insmod src/rootkit.ko
dmesg | tail
```

## Verify

After loading, `mkdir` calls will be blocked:

```
mkdir /tmp/test_keyneyloggins
# Should fail silently - check dmesg for "[+] mkdir be hooked" message
```

## Unload

```
sudo rmmod rootkit
```
