#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/utsname.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A test rootkit");

char * get_map_file(void) {

  int MAX_FILESIZE;
  MAX_FILESIZE = 100;
  char base_filename[] =  "/boot/System.map-";
  char * kernel_release = utsname()->release;
  char * kernel_release_buff = kmalloc(MAX_FILESIZE, GFP_KERNEL);

  //char * strncat (char * dest, const char * src, size_t count);
  strncpy(kernel_release_buff, base_filename, sizeof(base_filename));
  strncat (kernel_release_buff, kernel_release, 99);

  printk(KERN_WARNING "Kernel release %s\n", kernel_release_buff);

  return kernel_release_buff;
}

int find_sys_call_table(char * map_filename, size_t map_filename_size) {

  struct file *f;

  mm_segment_t oldfs;
  oldfs = get_fs();
  set_fs(KERNEL_DS)

  print(KERN_WARNING "%s\n", map_filename);

}


void free_map_file(char * map_file) {
  kfree(map_file);
}


static int __init hello_init(void) {

  get_map_file();
  return 0;
}


static void __exit hello_cleanup(void) {
  printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
