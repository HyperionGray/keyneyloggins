# Architecture

## Syscall Table Hooking

The rootkit operates by modifying the kernel's system call table at runtime.

### Module Load Sequence

1. `root_init()` is called by the kernel when the module is loaded via `insmod`
2. `get_map_file()` constructs the path to `/boot/System.map-<kernel_version>`
3. `find_sys_call_table()` reads the System.map file line by line, searching for `sys_call_table`
4. The hex address is parsed via `kstrtoul()` and stored in the global `syscall_table` pointer
5. Write protection is disabled by clearing bit 16 of CR0
6. The original `mkdir` handler is saved and replaced with `new_mkdir`
7. Write protection is re-enabled

### Module Unload Sequence

1. `root_cleanup()` is called by the kernel when the module is removed via `rmmod`
2. Write protection is disabled
3. The original `mkdir` handler is restored from the saved pointer
4. Write protection is re-enabled

### Known Limitations

- Relies on `/boot/System.map-*` being present and readable
- Uses deprecated `get_fs()`/`set_fs()` API (removed in newer kernels)
- CR0 manipulation is x86-specific
- No hiding of the module itself (visible in `lsmod`)
- No process hiding or file hiding implemented yet

## Future Directions

- Module hiding (remove from module list)
- Process hiding (hook `getdents`/`getdents64`)
- File hiding
- Port forwarding / reverse shell capability
- Keylogging (as the name implies)
- Modern kernel compatibility (5.x+ without `set_fs`)
