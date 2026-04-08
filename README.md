# keyneyloggins

A Linux kernel rootkit wireframe, originally presented at **CarolinaCon 2018** by Hyperion Gray.

This is an educational/research project demonstrating syscall table hooking in the Linux kernel. It locates the `sys_call_table` by parsing `/boot/System.map-*` and hooks the `mkdir` syscall as a proof of concept.

## Repository Structure

```
src/            Kernel module source and Makefile
testing/        Test utilities (uname helper, syscall table lookup)
docs/           Design notes and documentation
reload.sh       Quick module reload script
Makefile        Top-level build (delegates to src/)
```

## Building

Requires kernel headers for the running kernel:

```bash
# Install headers (Debian/Ubuntu)
sudo apt install linux-headers-$(uname -r)

# Build
make build

# Clean
make clean
```

## Usage

**WARNING**: This is a kernel module that hooks syscalls. Only use in a VM or test environment.

```bash
# Load the module
sudo insmod src/rootkit.ko

# Check kernel log
dmesg | tail

# Reload (unload + load)
./reload.sh

# Unload
sudo rmmod rootkit
```

## How It Works

1. Reads `/boot/System.map-$(uname -r)` to locate `sys_call_table`
2. Disables write protection on the syscall table (CR0 bit 16)
3. Replaces the `mkdir` syscall handler with a hooked version
4. Restores the original handler on module unload

See `docs/design.md` for architecture details.

## License

GPL (as required for Linux kernel modules).
