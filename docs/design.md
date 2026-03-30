# keyneyloggins -- Design Notes

## Overview

keyneyloggins is a proof-of-concept Linux kernel rootkit that demonstrates syscall table hooking. It was presented at CarolinaCon 2018.

## Architecture

### Module Lifecycle

1. **`root_init()`** -- Called on `insmod`. Locates the syscall table, disables CR0 write-protect, swaps the `__NR_mkdir` entry, re-enables write-protect.
2. **`root_cleanup()`** -- Called on `rmmod`. Restores the original syscall handler.

### Syscall Table Discovery

The module finds `sys_call_table` at runtime by:

1. Building the System.map path: `/boot/System.map-<kernel_release>`
2. Opening and scanning the file line-by-line in kernel space (`filp_open` / `vfs_read`)
3. Parsing the hex address from the matching line via `kstrtoul`

This avoids hardcoded addresses and works across kernel versions (where System.map is present).

### Write-Protect Bypass

The syscall table is in read-only memory. The module clears CR0 bit 16 (WP) to allow writes:

```
write_cr0(read_cr0() & (~0x10000));   // disable WP
// ... modify syscall table ...
write_cr0(read_cr0() | 0x10000);      // re-enable WP
```

### Current Hook

`__NR_mkdir` is replaced with `new_mkdir()`, which logs via `printk` and returns 0 (silently drops the mkdir).

## Known Limitations / TODO

- The cleanup handler restores `__NR_write` instead of `__NR_mkdir` (bug in `root_cleanup`)
- Uses deprecated `set_fs(KERNEL_DS)` / `get_fs()` APIs (removed in newer kernels)
- `vfs_read` for file I/O in kernel space is discouraged in modern kernels
- No hiding of the module itself (visible in `lsmod`)
- Only hooks `mkdir`; could be extended to other syscalls

## Future Directions

- Port to modern kernel APIs (5.x+) using kprobes or ftrace-based hooking
- Add module hiding (remove from `lsmod` / `/proc/modules`)
- Add process hiding
- Add file hiding (directory entry filtering)
- Add network connection hiding
