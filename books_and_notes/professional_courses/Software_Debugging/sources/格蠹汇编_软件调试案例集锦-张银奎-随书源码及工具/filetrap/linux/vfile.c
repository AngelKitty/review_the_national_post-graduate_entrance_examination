

int     fd_sysfs_vfile = -1;  

fd_sysfs_vfile=open(BATT_SYSFS_SOC, O_BINARY|O_RDWR);

int send_to_driver()
{
    int p,d,ret;
    char hex[3];
    if(fd_sysfs_vfile==-1)
        return -1;
    
    p = g_last_soc*100.0;
    d = (p/16);
    hex[0] = d<10?(d+48):(d+55);
    
    d = (p%16);
    hex[1] = d<10?(d+48):(d+55);
    hex[2] = 0;
    lseek(fd_sysfs_vfile, 0, SEEK_SET);
    ret = write(fd_sysfs_vfile,hex,3);
    if(ret<0)
    {  
       d4d(LOG_FATAL, "write sysfs failed with %d\n", ret);
    }
    // flush(fd_sysfs_vfile);

    return ret;  
}

    d4d(LOG_FATAL, "submit %d(%s) to file %d (log handle %d) ret=%d\n", 
            p, hex, fd_sysfs_vfile, fptr_log, ret);


static ssize_t
vfile_store(struct kobject *kobj, struct kobj_attribute *attr,
					const char *buf, size_t n)
{
	unsigned long percent = simple_strtoul(buf, NULL, 0x10);

	if (percent > 100)
		g_vfile_pecent = 100;
	else
		g_vfile_pecent = percent;

	return n;
}

