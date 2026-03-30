# TODO

## High Priority

- Port to modern kernel APIs: `set_fs`/`get_fs` and `vfs_read` from userspace are removed in kernel 5.10+. Use `kernel_read` or `filp_open` + `kernel_read` instead.
- Replace CR0-based write protection bypass: Modern kernels (5.3+) use `write_cr0` pinning. Investigate alternatives (e.g., `set_memory_rw`/`set_memory_ro` or `probe_kernel_write`).
- Use `kallsyms_lookup_name` to find `sys_call_table` instead of parsing System.map from disk.

## Medium Priority

- Add keylogger functionality (per project name "keyneyloggins") by hooking `__NR_read` or using keyboard notifier.
- Add process hiding capability.
- Add module hiding (remove from `lsmod` output).
- Add proper error handling and return codes throughout.
- Split large functions into smaller units and move declarations to `include/` headers.

## Low Priority

- Add a `VERSION` file for tracking releases.
- Add CI to at least validate that the module compiles against a known kernel version.
- Add `testing/` integration tests that load/unload in a VM.
- Clean up `testing/uname.c` - add proper error handling and remove commented-out kernel code.
