#include <linux/rbtree.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/moduleparam.h>
#include <asm/uaccess.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/ioctl.h>
#include <linux/version.h>
#include "rbt_header.h"
#include <linux/uaccess.h>

#define DEVICE_NAME "rbt530_dev"  // device name to be created and registered

static dev_t rb_dev_number;
static struct class *rb_dev_data;
static struct device *rb_dev_device;
struct rb_object;
struct rb_dev;

static int __init rbt530_driver_init(void);
static void __exit rbt530_driver_exit(void);
static int rb_open(struct inode *, struct file *);
static int rb_release(struct inode *, struct file *);
static ssize_t rb_read(struct file *, char *, size_t, loff_t *);
static ssize_t rb_write(struct file *, const char *, size_t, loff_t *);
struct rb_node *rb_search(struct rb_root *, int);
static int rb_insert(struct rb_root *, struct rb_object * );
static long rb_ioctl(struct file *, unsigned int , unsigned long);

static DEFINE_MUTEX(rb_mutex);

typedef struct rb_object {
	struct rb_node my_node;
	int key;
	int data;
}rb_object_t;

struct rb_dev {
        struct cdev cdev;               /* The cdev structure */
        char name[20];                  /* Name of device*/
	char input[2];
	char output[100];
	struct rb_root rbt530;
	long set_cmd;
	struct rb_node *curr;
}*rb_devp0,*rb_devp1;


int rb_open(struct inode *inode, struct file *file)
{
	
	struct rb_dev *rb_devp;
	//mutex lock
	mutex_lock(&rb_mutex);
	
	

	printk("RB: opening\n");
		
	// Get the per-device structure that contains this cdev */
	rb_devp = container_of(inode->i_cdev, struct rb_dev, cdev);
		
	//Now we can access the driver struct elements.
	file->private_data = rb_devp;
	printk("RB : %s is opening \n", rb_devp->name);
	return 0;
}

 
/*
  * Release the driver
  */
int rb_release(struct inode *inode, struct file *file)
{
	struct rb_dev *rb_devp = file->private_data;
	printk("%s is closing\n", rb_devp->name);
	
	//release mutex.
	mutex_unlock(&rb_mutex);
	return 0;
}

/*
 * IOCTL FOR READING SET_CMD VALUE.
 */
static long rb_ioctl(struct file *file, unsigned int ioctl_num, unsigned long val)
{
	struct rb_dev *rb_devp = file->private_data;
	int temp;
	printk(KERN_INFO "RB : IOCTL CALLED...");

	
       
        
	switch(ioctl_num)
	{
		case IOCTL_SET_END:
			temp = (int) val;
			rb_devp->set_cmd = temp;
			break;		
		default:
			return -EINVAL;
	}
	
	printk(KERN_INFO "RB :set_cmd  = %ld \n",rb_devp->set_cmd);
	return 0;
}


/*
 WRITE TO THE TREE.
 */
ssize_t rb_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	int keyVal=0, dataVal=0;
	struct rb_object *my_rbtree;
	struct rb_dev *rb_devp = file->private_data;
	struct rb_node *searchVal;
        
	int wr_err=0; 
	wr_err = copy_from_user(rb_devp->input,buf,count);
	
	if(wr_err!=0){
		printk(KERN_INFO "RB :Error while writing data.\n");
		return -1;
	}
	
	
	my_rbtree = (struct rb_object *)kmalloc(sizeof(struct rb_object), GFP_KERNEL);
	keyVal = rb_devp->input[0];
	dataVal = rb_devp->input[1];
	
	
	my_rbtree->key = keyVal;
	my_rbtree->data = dataVal;
	
	printk(KERN_INFO "RB :keyval = %d, dataVal = %d",keyVal, dataVal);
	printk(KERN_INFO "RB :Search if keyval exists... ");
	searchVal = rb_search(&rb_devp->rbt530,keyVal);

	//SEARCH IF THE KeyVALUE EXISTS.
	
	
        if(searchVal == NULL)
		printk("RB : Key was not found in the tree");	

	if(searchVal){
		printk(KERN_INFO "RB : Key was found in tree");
		if(dataVal != 0) {
			printk(KERN_INFO "RB : Node replaced");
			rb_replace_node(searchVal, &my_rbtree->my_node, &rb_devp->rbt530);
		}
		else{
			//Erase a node.
			rb_erase(searchVal,&rb_devp->rbt530);
			printk(KERN_INFO "RB :Node is erased.");
		}
	}
	else{
		printk(KERN_INFO "RB : Node inserting %d ... \n",keyVal);	
		rb_insert(&rb_devp->rbt530, my_rbtree);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(rb_write);


//Search a node
struct rb_node *rb_search(struct rb_root *root, int keytoFind){


	struct rb_node *walker = root->rb_node;

	while(walker){
	
		rb_object_t *structdata = container_of(walker, rb_object_t, my_node);
		
		if(keytoFind < structdata->key)
			walker = walker->rb_left;
		else if (keytoFind > structdata->key)
			walker = walker->rb_right;
		else
			return &structdata->my_node;
		}
	return NULL;
}


//Insert a node
int rb_insert(struct rb_root *root, struct rb_object *structdata){

	struct rb_node **new = &(root->rb_node), *parent=NULL;
	while(*new){
	
		struct rb_object *this = container_of(*new, rb_object_t, my_node);	
		int cmpresult= structdata->key > this->key ? 1 : 0;	
		parent = *new;
			
		if(cmpresult == 0)
			new =&((*new)->rb_left);
		else
			new= &((*new)->rb_right);
	
	}

	
	rb_link_node(&structdata->my_node, parent, new);
	rb_insert_color(&structdata->my_node,root);
	
	//printk("RB :Node inserted %d, parent is %d",structdata->key,this->key);
	printk("RB :Node inserted ##");

	return 0;

//EXPORT_SYMBOL_GPL(parent);

}



//Read a node value
ssize_t rb_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	
        struct rb_dev *rb_devp = file->private_data;
	rb_object_t *structdata; 
	int result=0;
	printk(KERN_INFO "RB : Reading for set_cmd = %ld",rb_devp->set_cmd);

	if(rb_devp->set_cmd==1){
		
                
                if(rb_devp->curr==NULL)
		{
			
			if(rb_last(&(rb_devp->rbt530))==NULL)
			{
				printk("Reading failed! - tree empty");
				return -1;
			}
			rb_devp->curr = rb_last(&(rb_devp->rbt530));
		}
		else
		{
			if(rb_prev(rb_devp->curr)==NULL)
			{
				printk("Reading failed! - next object is NULL");
				return -1;
			}
			rb_devp->curr = rb_prev(rb_devp->curr);
		}
		//return its value
	        structdata = container_of(rb_devp->curr, rb_object_t, my_node);
		result = structdata->key;
		printk(KERN_INFO "RB :Read next value (descending order) = %d", result);
	
	}
	else if(rb_devp->set_cmd==0){
		
	  	if(rb_devp->curr==NULL)
		{
			if(rb_first(&(rb_devp->rbt530))==NULL)
			{
				printk("Reading failed! - tree empty");
				return -1;
			}
			rb_devp->curr = rb_first(&(rb_devp->rbt530));
		}
		else
		{	if(rb_next(rb_devp->curr)==NULL)
			{
				printk("Reading failed! - next object is NULL");
				return -1;
			}
			rb_devp->curr = rb_next(rb_devp->curr);
		}
		
        	structdata = container_of(rb_devp->curr, rb_object_t, my_node);
        	result = structdata->key;
		printk(KERN_INFO "RB :Read next value (ascending order) = %d", result);
	}
	
	else if(rb_devp->set_cmd==2){
		int start =0,k;
		struct rb_node *node;
		int bytes_read = 0;
		//Read all data from the tree by traversing
		
		printk("------------------------\n");
		printk("Reading complete tree:\n");
		////////////////////////////////////////////
		
		
		
  		for (node = rb_first(&(rb_devp->rbt530)); node; node = rb_next(node)){
  			k = rb_entry(node, struct rb_object, my_node)->key;
  			rb_devp->output[start]=k;
			printk("key #%d =%d\n", start, k);
			start++;
		}
		printk(KERN_INFO "RB : All tree data collected \n");
		
        	while (count && rb_devp->output[bytes_read]) {
			put_user(rb_devp->output[bytes_read], buf++);
			count--;
			bytes_read++;
		}
        	
		printk(KERN_INFO "RB : ALL Data read done.");
		result = bytes_read;
		printk("-------------------------\n");
		/////////////////////////////////////////////
	}
	else
		return - 1;

	return result;
}

EXPORT_SYMBOL_GPL(rb_read);

/* File operations structure. Defined in linux/fs.h */
static struct file_operations rb_fops = {
	.owner              = THIS_MODULE,           /* Owner */
        .open               = rb_open,        /* Open method */
	.unlocked_ioctl     = rb_ioctl,     /* IOCTL for driver rbtree */
        .release            = rb_release,     /* Release method */
        .write              = rb_write,       /* Write method */
        .read               = rb_read,        /* Read method */
};



int __init rbt530_driver_init(void)
{
        int ret;
	int major;
	dev_t my_device;

        /* Request dynamic allocation of a device major number */
        if (alloc_chrdev_region(&rb_dev_number, 0, 2, DEVICE_NAME) < 0) {
                        printk(KERN_DEBUG "RB :Can't register device\n"); return -1;
        }
	
	major = MAJOR(rb_dev_number);
	
        /* Populate sysfs entries */
        rb_dev_data = class_create(THIS_MODULE, DEVICE_NAME);
	
	my_device = MKDEV(major,0);
	
        /* Allocate memory for the per-device structure */
        rb_devp0 = kmalloc(sizeof(struct rb_dev), GFP_KERNEL);

        if (!rb_devp0) {
                printk(KERN_INFO "RB :Bad Kmalloc\n"); return -ENOMEM;
        }

        /* Request I/O region */
        sprintf(rb_devp0->name, DEVICE_NAME);

        /* Connect the file operations with the cdev */
        cdev_init(&rb_devp0->cdev, &rb_fops);
        rb_devp0->cdev.owner = THIS_MODULE;

        /* Connect the major/minor number to the cdev */
        ret = cdev_add(&rb_devp0->cdev, (my_device), 1);

        if (ret) {
                printk("Bad cdev\n");
                return ret;
        }
        
	/* Send uevents to udev, so it'll create /dev nodes */
        rb_dev_device = device_create(rb_dev_data, NULL, my_device, NULL, "rbt530_dev0");    

        rb_devp0->rbt530=RB_ROOT;
        rb_devp0->curr = NULL;
        
        printk("RB :device 0 initialized.\n");
        
        //---------------------------------------------------------------------------
        my_device = MKDEV(major,1);
	
        /* Allocate memory for the per-device structure */
        rb_devp1 = kmalloc(sizeof(struct rb_dev), GFP_KERNEL);

        if (!rb_devp1) {
                printk(KERN_INFO "RB :Bad Kmalloc\n"); return -ENOMEM;
        }

        /* Request I/O region */
        sprintf(rb_devp1->name, DEVICE_NAME);

        /* Connect the file operations with the cdev */
        cdev_init(&rb_devp1->cdev, &rb_fops);
        rb_devp1->cdev.owner = THIS_MODULE;

        /* Connect the major/minor number to the cdev */
        ret = cdev_add(&rb_devp1->cdev, (my_device), 1);

        if (ret) {
                printk("Bad cdev\n");
                return ret;
        }
        
	/* Send uevents to udev, so it'll create /dev nodes */
        rb_dev_device = device_create(rb_dev_data, NULL, my_device, NULL, "rbt530_dev1");    

        rb_devp1->rbt530=RB_ROOT;
        rb_devp1->curr = NULL;
        //----------------------------------------------------------------------------
        
        printk("RB :device 1 initialized.\n");

        printk("RB :Tree driver initialized with 2 devices.\n");
        return 0;
}


void __exit rbt530_driver_exit(void)
{
        /* Release the major number */
        unregister_chrdev_region((rb_dev_number), 2);
		
        /* Destroy device */
        device_destroy (rb_dev_data, MKDEV(MAJOR(rb_dev_number), 0));
        cdev_del(&rb_devp0->cdev);
        kfree(rb_devp0);
        printk("RB : rbt530_dev0 device removed.\n");
	
	//--------------------------
	device_destroy (rb_dev_data, MKDEV(MAJOR(rb_dev_number), 1));
        cdev_del(&rb_devp1->cdev);
        kfree(rb_devp1);
        printk("RB : rbt530_dev1 device removed.\n");
	//--------------------------
        /* Destroy driver_class */
        class_destroy(rb_dev_data);
			
        printk("RB : rbt530_dev driver removed.\n");
}


module_init(rbt530_driver_init);
module_exit(rbt530_driver_exit);
MODULE_LICENSE("GPL v2");

