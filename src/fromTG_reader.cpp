
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#define __USE_BSD
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string.h>

#include "logger.hpp"

int main()
{
    int rc=0;
    int fromTG = open("fromTG.fifo", O_RDONLY);
    if (fromTG < 1)
    {
        printf("failed to open fromTG");
        exit(-1);
    }

    fd_set fd_in;
    char buf[150];

    while (1)
    {
        FD_ZERO(&fd_in);
        FD_SET(fromTG, &fd_in);

        rc = select(fromTG + 1, &fd_in, NULL, NULL, NULL);

        if(FD_ISSET(fromTG, &fd_in))
        {
            rc = read(fromTG, buf, sizeof(buf)-1);
            if(rc>0) std::cout<< "\n > " << buf;
        }
    }
    exit(1);
}