#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 
#include <linux/fs.h> 
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pariwat Leamsumran <leamsumran.p@gmail.com>"); 
MODULE_DESCRIPTION("A simple Linux driver for i2c ex."); 
MODULE_VERSION("0.1"); 

#define PLATFORM_NAME "i2c_expander_driver"

static struct kobject *i2c_kobject;
static int foo;

/*************** Sysfs Fuctions **********************/
static ssize_t sysfs_show(struct kobject *kobj, 
                struct kobj_attribute *attr, char *buf);
static ssize_t sysfs_store(struct kobject *kobj, 
                struct kobj_attribute *attr,const char *buf, size_t count);
struct kobj_attribute foo_attribute = __ATTR(foo, 0660, sysfs_show, sysfs_store);


static ssize_t sysfs_show(struct kobject *kobj, 
                struct kobj_attribute *attr, char *buf)
{
        printk(KERN_INFO "Sysfs - Read!!!\n");
        return sprintf(buf, "%d", foo);
}

static ssize_t sysfs_store(struct kobject *kobj, 
                struct kobj_attribute *attr,const char *buf, size_t count)
{
        printk(KERN_INFO "Sysfs - Write!!!\n");
        sscanf(buf,"%d",&foo);
        return count;
}

// static struct kobj_attribute foo_attribute =__ATTR(foo, 0660, foo_show,foo_store);

static int i2c_platform_device_probe(struct platform_device *pdev)
{
    int error = 0;

    printk(KERN_INFO "Module initialized successfully \n");

    i2c_kobject = kobject_create_and_add("kobject_example",
                                         kernel_kobj);
    if(!i2c_kobject)
        return -ENOMEM;

    error = sysfs_create_file(i2c_kobject, &foo_attribute.attr);
    if (error) {
        pr_debug("failed to create the foo file in /sys/kernel/kobject_example \n");
    }
	return 0;
}

static int i2c_platform_device_remove(struct platform_device *pdev)
{
    printk(KERN_INFO "Module un initialized successfully \n");
    kobject_put(i2c_kobject);
  	return 0;
}

static struct platform_driver i2c_expander_plaform_driver = {
    .probe      = i2c_platform_device_probe,
    .remove     = i2c_platform_device_remove,
    .driver     = {
        .name   = PLATFORM_NAME,
        .owner = THIS_MODULE,
    },
};

static int i2c_expander_platform_device_init(void)
{
    printk(KERN_INFO "i2c_expander_platform_device_init\n");
    return platform_driver_register(&i2c_expander_plaform_driver);
}

static void i2c_expander_platform_device_exit(void)
{
    printk(KERN_INFO "i2c_expander_platform_device_exit\n");
    platform_driver_unregister(&i2c_expander_plaform_driver);
}

module_init(i2c_expander_platform_device_init)
module_exit(i2c_expander_platform_device_exit)