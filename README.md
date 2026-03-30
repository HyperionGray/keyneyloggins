# keyneyloggins

A Linux kernel module that demonstrates syscall table hooking. Currently hooks `mkdir` to intercept directory creation calls.

## Repository Structure

```
src/          - kernel module source code
include/      - header files
build/        - compiled kernel objects (.ko)
bin/          - helper scripts (reload, etc.)
testing/      - test utilities and scripts
docs/         - project documentation
```

## Requirements

- Linux kernel headers for your running kernel
- GNU Make
- GCC with kernel module support

Install on Debian/Ubuntu:
```
sudo apt install linux-headers-$(uname -r) build-essential
```

## Building

```
make build
```

The compiled `rootkit.ko` will be placed in `build/`.

## Loading / Unloading

```
sudo insmod build/rootkit.ko
sudo rmmod rootkit
```

Or use the helper script:
```
bin/reload.sh
```

Check kernel log for output:
```
dmesg | tail
```

## How It Works

1. On load, the module reads `/boot/System.map-$(uname -r)` to locate the `sys_call_table` address
2. Disables write protection on the syscall table via CR0
3. Replaces the `mkdir` syscall with a custom handler
4. On unload, restores the original syscall handler

## Security Note

This is an educational/research project. Loading kernel modules requires root privileges and can destabilize your system. Use in a VM or test environment only.
