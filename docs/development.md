# Development Notes

## Architecture

The module operates by:
1. Reading the kernel's System.map file to find the `sys_call_table` address
2. Disabling CR0 write protection to make the syscall table writable
3. Replacing target syscall entries with custom handlers
4. Re-enabling write protection

## Files

- `src/rootkit.c` - Main kernel module source. Contains init/cleanup, syscall table lookup, and hook functions.
- `testing/uname.c` - Userspace utility to print kernel version info.
- `testing/get_table_address.sh` - Shell script to find syscall table address from System.map.
- `bin/reload.sh` - Helper to unload and reload the module during development.

## Building

The kernel build system (kbuild) is invoked from the top-level Makefile. Source lives in `src/`, compiled `.ko` output goes to `build/`.

## Known Limitations

- Only works on older kernels (pre-5.10) due to `set_fs`/`get_fs` usage
- CR0 write protection bypass is blocked on kernels 5.3+ with pinned CR0
- System.map parsing is fragile; `kallsyms_lookup_name` would be more robust
