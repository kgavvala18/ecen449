
#include <linux/module.h>      /* Needed by all modules */
#include <linux/moduleparam.h> /* Needed for module parameters */
#include <linux/kernel.h>      /* Needed for printk and KERN_* */
#include <linux/init.h>        /* Need for __init macros  */
#include <linux/fs.h>          /* Provides file ops structure */
#include <linux/sched.h>       /* Provides access to the "current" process
task structure */
#include <linux/slab.h>        //needed for kmalloc() and kfree()
#include <asm/io.h>            //needed for IO reads and writes
#include <asm/uaccess.h>       // Provides utilities to bring user space

#include "xparameters.h" //needed for physical address of the multiplier

#define DEVICE_NAME "multiplier"
#define BUF_LEN 80

#define PHY_ADDR XPAR_MULTIPLY_0_S00_AXI_BASEADDR // physical address of multiplier
// size of physical address range for multiply
#define MEMSIZE XPAR_MULTIPLY_0_S00_AXI_HIGHADDR - XPAR_MULTIPLY_0_S00_AXI_BASEADDR + 1

/* Function prototypes, so we can setup the function pointers for dev
file access correctly. */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static int Major;
/ Major number assigned to our device driver * /
    static int Device_Open = 0; // Flag to signify open device

void *virt_addr; // virtual address pointing to multiplier

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release};

// initialize the module
// called when the module is loaded
static int __init my_init(void)
{
    printk(KERN_INFO "Mapping virutal address...\n");
    // map virtual address to multiplier physical address//use ioremap
    virt_addr = ioremap(PHY_ADDR, MEMSIZE);
    printk("Physical Address: 0x%x\n", PHY_ADDR);
    printk("Virtual Address: 0x%x\n", virt_addr);

    Major = register_chrdev(0, DEVICE_NAME, &fops); // dynamic allocation

    /* Negative values indicate a problem */
    if (Major < 0)
    {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "Registered a device with dynamic Major number of %d\n", Major);
    printk(KERN_INFO "Create a device file for this device with this command:\n'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);

    return 0; /* success */
}

// called when the module is unloaded
// called when the module is removed
static void __exit my_cleanup(void)
{
    // Unregister the device
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_ALERT "unmapping virtual address space...\n");
    iounmap((void *)virt_addr);
}

// called when a process opens the device file
static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "Device has been opened\n");
    return 0;
}

// called when a process closes the device file
static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "Device has been closed\n");
    return 0;
}

// called when a process reads from the device file
static ssize_t device_read(struct file *filp,
                           char *buffer,
                           size_t length,
                           loff_t *offset)
{

    // bytes read from the buffer
    int bytes_read = 0;
    // allocating kernel buffer
    int *kBuff = (int *)kmalloc(length * sizeof(int), GFP_KERNEL);
    kBuff[0] = ioread32(virt_addr);
    kBuff[1] = ioread32(virt_addr + 4);
    kBuff[2] = ioread32(virt_addr + 8);
    char *kBuffer = (char *)kBuff; // bytes written one at a time

    int i;
    for (i = 0; i < length; i++)
    {                                     // read the buffer one byte at a time
        put_user(*(kBuffer++), buffer++); // char is one byte
        // put_user copies the data from kernel space to user space
        // buffer is the user space buffer
        // kBuffer is the kernel space buffer

        bytes_read++;
    }

    kfree(kBuff);

    // printk("bytes_read: %d\n", bytes_read);
    return bytes_read;
}

static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    char *kBuff = (char *)kmalloc((len + 1) * sizeof(char), GFP_KERNEL);
    /* use kBuff to write one
        byte at a time from user buffer*/
    int i;
    for (i = 0; i < len; i++)
    {
        get_user(kBuff[i], buff++);
    }

    kBuff[len] = '\0';
    /* Convert kBuff to int* to write to
        multiply*/
    int *writeBuffer = (int *)kBuff;

    // write to register 0
    printk(KERN_INFO "Writing %d to register 0\n", writeBuffer[0]);
    iowrite32(writeBuffer[0], virt_addr + 0); // base address + offset

    // write to register 1
    printk(KERN_INFO "Writing %d to register 1\n", writeBuffer[1]);
    iowrite32(writeBuffer[1], virt_addr + 4);

    kfree(writeBuffer);
    return i; // number of bytes written
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kris Gavvala");
MODULE_DESCRIPTION("Multiplier Device Driver");

module_init(my_init);
module_exit(my_cleanup);
