/*  multiply.c - The simplest kernel module.
 

*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>

#include "xparameters.h"

#define PHYS_ADDR XPAR_MULTPLY_0_S00_AXI_BASEADDR

#define MEMSIZE XPAR_MULTPLY_0_S00_AXI_HIGHADDR - XPAR_MULTPLY_0_S00_AXI_BASEADDR+1

void * virt_addr; //virtual address pointing to multiplier


static int __init my_init(void)
{
	printk(KERN_INFO "Mapping virtual address...\n");
	
	//print the physical and virtual adress of multiplication peripheral.
	
	printk("physical address:");
	
	printk(PHYS_ADDR); printk("\n");
	
	printk("virtual address:");
	
	printk(virt_addr); printk("\n");
	
	printk(KERN_INFO "Writing a 7 to register 0\n");
	
	iowrite32( 7, virt_addr+0);
	
	printk(KERN_INFO "Writing a 2 to register 1\n");
	
	iowrite32( 2, virt_addr+4);
	
	printk("Read %d from register 0\n", ioread32(virt_addr+0));
	printk("Read %d from register 1\n", ioread32(virt_addr+4));
	printk("Read %d from register 2\n", ioread32(virt_addr+8));

	return 0;
}

/* run this function just prior to the modules removal from the system. you should release all resources used by your module here
*/
static void __exit my_exit(void)
{
	
	printk(KERN_ALERT "unmapping virtual adress space...\n");
	iounmap((void*)virt_addr);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ECEN449 Student: Kris Gavvala");
MODULE_DESCRIPTION("Simple Hello world module");

module_init(my_init);
module_exit(my_exit);
