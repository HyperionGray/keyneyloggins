# keyneyloggins

A Linux kernel rootkit wireframe as presented at CarolinaCon 2018 by Hyperion Gray.

This module demonstrates syscall table hooking on Linux by intercepting the `mkdir` system call via the kernel module interface. It locates the `sys_call_table` by parsing `/boot/System.map-*` and replaces the `mkdir` handler with a custom function.

**WARNING**: This is an educational/research tool. Loading kernel modules that modify the syscall table can crash your system. Use only in isolated VMs or test environments.

## Directory Structure

```
src/           - Kernel module source code
include/       - Header files (reserved for future use)
bin/           - Helper scripts (reload, diagnostics)
build/         - Build output (generated, gitignored)
docs/          - Documentation
testing/       - Userspace test utilities
Makefile       - Kernel module build system
```

## Building

Requires Linux kernel headers for your running kernel:

```
# Install kernel headers (Debian/Ubuntu)
sudo apt install linux-headers-$(uname -r)

# Build the module
make build

# Clean build artifacts
make clean
```

## Usage

```
# Load the module
sudo insmod src/rootkit.ko

# Check kernel log for hook confirmation
dmesg | tail

# Unload the module
sudo rmmod rootkit

# Or use the reload helper
./bin/reload.sh
```

## How It Works

1. On load (`root_init`), the module reads `/boot/System.map-$(uname -r)` to find the `sys_call_table` address
2. It disables write protection on the syscall table via CR0 register manipulation
3. It replaces the `__NR_mkdir` entry with `new_mkdir`, which blocks all `mkdir` calls
4. On unload (`root_cleanup`), it restores the original `mkdir` handler

## License

GPL
