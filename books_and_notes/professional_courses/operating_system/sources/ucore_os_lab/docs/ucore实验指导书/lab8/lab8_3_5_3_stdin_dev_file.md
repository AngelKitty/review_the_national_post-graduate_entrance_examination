
#### 3.5.3 stdin 设备文件 

这里的stdin设备文件实际上就是指的键盘。这个设备文件是一个只读设备，如果写这个设备，就会出错。接下来我们看看stdin设备的相关处理过程。

**初始化**

stdin设备文件的初始化过程主要由stdin\_device\_init完成了主要的初始化工作，具体实现如下：

```
static void
stdin_device_init(struct device *dev) {
    dev->d_blocks = 0;
    dev->d_blocksize = 1;
    dev->d_open = stdin_open;
    dev->d_close = stdin_close;
    dev->d_io = stdin_io;
    dev->d_ioctl = stdin_ioctl;

    p_rpos = p_wpos = 0;
    wait_queue_init(wait_queue);
}
```

相对于stdout的初始化过程，stdin的初始化相对复杂一些，多了一个stdin\_buffer缓冲区，描述缓冲区读写位置的变量p\_rpos、p\_wpos以及用于等待缓冲区的等待队列wait\_queue。在stdin\_device\_init函数的初始化中，也完成了对p\_rpos、p\_wpos和wait\_queue的初始化。

**访问操作实现**

stdin\_io函数负责完成设备的读操作工作，具体实现如下：

```
static int
stdin_io(struct device *dev, struct iobuf *iob, bool write) {
    if (!write) {
        int ret;
        if ((ret = dev_stdin_read(iob->io_base, iob->io_resid)) > 0) {
            iob->io_resid -= ret;
        }
        return ret;
    }
    return -E_INVAL;
}
```

可以看到，如果是写操作，则stdin\_io函数直接报错返回。所以这也进一步说明了此设备文件是只读文件。如果此读操作，则此函数进一步调用dev\_stdin\_read函数完成对键盘设备的读入操作。dev\_stdin\_read函数的实现相对复杂一些，主要的流程如下：

```
static int
dev_stdin_read(char *buf, size_t len) {
    int ret = 0;
    bool intr_flag;
    local_intr_save(intr_flag);
    {
        for (; ret < len; ret ++, p_rpos ++) {
        try_again:
            if (p_rpos < p_wpos) {
                *buf ++ = stdin_buffer[p_rpos % stdin_BUFSIZE];
            }
            else {
                wait_t __wait, *wait = &__wait;
                wait_current_set(wait_queue, wait, WT_KBD);
                local_intr_restore(intr_flag);

                schedule();

                local_intr_save(intr_flag);
                wait_current_del(wait_queue, wait);
                if (wait->wakeup_flags == WT_KBD) {
                    goto try_again;
                }
                break;
            }
        }
    }
    local_intr_restore(intr_flag);
    return ret;
}
```

在上述函数中可以看出，如果p\_rpos < p\_wpos，则表示有键盘输入的新字符在stdin\_buffer中，于是就从stdin\_buffer中取出新字符放到iobuf指向的缓冲区中；如果p\_rpos \>=p\_wpos，则表明没有新字符，这样调用read用户态库函数的用户进程就需要采用等待队列的睡眠操作进入睡眠状态，等待键盘输入字符的产生。

键盘输入字符后，如何唤醒等待键盘输入的用户进程呢？回顾lab1中的外设中断处理，可以了解到，当用户敲击键盘时，会产生键盘中断，在trap\_dispatch函数中，当识别出中断是键盘中断（中断号为IRQ\_OFFSET + IRQ\_KBD）时，会调用dev\_stdin\_write函数，来把字符写入到stdin\_buffer中，且会通过等待队列的唤醒操作唤醒正在等待键盘输入的用户进程。
