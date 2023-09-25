#include<linux/kernel.h>
#include<linux/sched/signal.h>
#include<linux/moduleparam.h>
#include<linux/module.h>
#include<linux/init.h>

MODULE_LICENSE("madhav");
MODULE_AUTHOR("os:);

static char *process = "abcde";
module_param(process,charp,0);

void Info_Print(void)
{
    struct task_struct *list;
    for_each_process(list)
    {

        if (strstr(list->comm, process))
        {
            printk(KERN_INFO "path : %s\n", list->comm);
            printk(KERN_INFO "pid : %d\n", list->pid);
            printk(KERN_INFO "user_id : %d\n", list->cred->uid);
            printk(KERN_INFO "pgid : %d\n", list->group_leader->pid);

        }
    }
}

static int Start(void)
{

    Info_Print();
    return 0;
}

static void End(void)
{

    printk(KERN_INFO "Module removed\n");
}

module_init(Start);
module_exit(End);
