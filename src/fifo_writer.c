#include "fifo.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main()
{
    int test_fifo = open("test_fifo", O_WRONLY);
    if (test_fifo < 1)
    {
        printf("failed to open fifo");
        exit(-1);
    }
    else
    {
        printf("--fds = %d", test_fifo);
    }

    char buf[BUF_SIZE];
    char buf2[BUF_SIZE];
    int rc = 0;

    printf("\nReady for inputs...");
    for (;;)
    {
        printf("\n $> ");
        rc = read(1, buf, BUF_SIZE);

        if (rc > 0)
        {
            printf("read %d characters from stdout",rc);
            int i=0;
            for(int i=0;i<rc;i++)
            {
                printf(" %c : %d\n",buf[i],(int)buf[i]);
                buf2[i] = buf[i];
            }
            printf("\nwriting %s",buf);
            write(test_fifo,buf,BUF_SIZE);
            for(int i=0;i<BUF_SIZE;i++)buf[i]=0;
        }
    }
    exit(0);
}