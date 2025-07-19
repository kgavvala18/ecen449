/*  hello.c - hello world kernel module.
  demonstrates  module instantiation , module release and printk. 
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>



/* this function is run upon module load. this is where you setup data structures and reserve resources used by the module
*/
static int __init my_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	//printk("<1>Module parameters were (0x%08x) and \"%s\"\n", myint,
	 //      mystr);

	//return platform_driver_register(&hello_driver);
	return 0;
}

/* this function is run just prior to the modules removal from the system. here you should release all resources used by your module (otherwise be prepared for reboot).
*/
static void __exit my_exit(void)
{
	//platform_driver_unregister(&hello_driver);
	printk(KERN_ALERT "Goodbye world!\n");
}

/* Standard module information, edit as appropriate */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ECEN449 Student: Kris Gavvala");
MODULE_DESCRIPTION("Simple Hello world module");

module_init(my_init);
module_exit(my_exit);
