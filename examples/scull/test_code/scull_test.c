#include <stdio.h>

#define LINUX_SYS_FUNC  1 
#define STANDARD_C_FUNC 0

#if LINUX_SYS_FUNC
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "scull_ioctl_cmd.h"
#include <sys/ioctl.h>
#endif /*LINUX_SYS_FUNC*/

int main(void)
{
    int i = 0, fd = 0;
    FILE * scull_f;
    int quantum = 0;

    printf("SCULL test program!\n");

#if LINUX_SYS_FUNC
    printf("Linux system I/O implement\n");
    fd = open("/dev/scull0", O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
    if(fd == -1){
        printf("Create/Open file error!\n");
        goto err;
    }

//    ioctl(fd, SCULL_IOCRESET);

    ioctl(fd, SCULL_IOCGQUANTUM, &quantum);
    printf("scull quantum = %d\n", quantum);

    for(i=0; i<10000000; i++)
    //while(1)
    {
        write(fd, "test", 5);
    }

    quantum = ioctl(fd, SCULL_IOCQQUANTUM);
    printf("scull quantum = %d\n", quantum);

err:
    close(fd);
#elif STANDARD_C_FUNC
    printf("Standard C file implement\n");
   //if((scull_f = fopen("/dev/scull0", "ab+")) == NULL) {
    if((scull_f = fopen("test.txt", "ab+")) == NULL) {
        printf("Open /dev/scull0 error\n");
        return -1;
    }
    for(i=0; i<10; i++) {
        fwrite("test", 4, 1, scull_f);
    }
    fclose(scull_f);
#else
    printf("Nothing to do!\n");
#endif

    return 0;
}
