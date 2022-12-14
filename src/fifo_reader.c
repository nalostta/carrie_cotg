#include "fifo.h"
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>



int main()
{
    int test_fifo = open("test_fifo", O_RDONLY);
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
    int rc = 0;

    printf("\nlistening now...");
    for (;;)
    {
        rc = read(test_fifo, buf, BUF_SIZE);

        if (rc > 0)
        {
            printf("\nread %d characters",rc);
            printf("\n > %s", buf);
        }
    }
    exit(0);
}