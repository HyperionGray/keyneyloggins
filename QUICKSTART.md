# Quickstart

## Prerequisites

- Linux system (x86_64) with kernel headers installed
- Root/sudo access
- **Use a VM** -- this hooks kernel syscalls

## Build & Run

```bash
# 1. Install kernel headers
sudo apt install linux-headers-$(uname -r)

# 2. Build the module
make build

# 3. Load it
sudo insmod src/rootkit.ko

# 4. Verify -- mkdir should be hooked
dmesg | tail
mkdir /tmp/test_hook   # Should fail silently

# 5. Unload
sudo rmmod rootkit
```

## Quick Reload

```bash
./reload.sh
```

This unloads and reloads the module, then shows the last 20 lines of `dmesg`.
