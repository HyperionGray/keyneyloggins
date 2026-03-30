# Quickstart

```
# Install dependencies
sudo apt install linux-headers-$(uname -r) build-essential

# Build the kernel module
make build

# Load the module
sudo insmod build/rootkit.ko

# Check kernel log
dmesg | tail

# Unload the module
sudo rmmod rootkit
```

See `README.md` for full details.
