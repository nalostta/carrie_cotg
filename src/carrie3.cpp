#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#define __USE_BSD
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <string.h>
#include <regex>

#include "logger.hpp"

int main(int ac, char *av[])
{
    int fdm, fds;
    int rc;
    char input[250];

    logger parent("logs/carrie3_parent.info.log");
    parent.log("start");

    int fromTG = open("../pipes/fromTG.fifo", O_RDONLY);
    if (fromTG < 1)
    {
        printf("failed to open fromTG");
        exit(-1);
    }
    parent.log("opened fromTG FIFO");

    int toTG = open("../pipes/toTG.fifo", O_WRONLY);
    if (toTG < 1)
    {
        printf("failed to open toTG");
        exit(-1);
    }

    parent.log("opened toTG FIFO");

    // Check arguments
    if (ac <= 1)
    {
        fprintf(stderr, "Usage: %s program_name [parameters]\n", av[0]);
        exit(1);
    }

    fdm = posix_openpt(O_RDWR);
    if (fdm < 0)
    {
        fprintf(stderr, "Error %d on posix_openpt()\n", errno);
        return 1;
    }

    rc = grantpt(fdm);
    if (rc != 0)
    {
        fprintf(stderr, "Error %d on grantpt()\n", errno);
        return 1;
    }

    rc = unlockpt(fdm);
    if (rc != 0)
    {
        fprintf(stderr, "Error %d on unlockpt()\n", errno);
        return 1;
    }

    // Open the slave side ot the PTY
    fds = open(ptsname(fdm), O_RDWR);

    parent.log("before fork");

    // Create the child process
    if (fork())
    {
        fd_set fd_in;

        // FATHER
        parent.log("Entered Parent thread");

        // Close the slave side of the PTY
        close(fds);

        while (1)
        {
            // Wait for data from standard input and master side of PTY
            FD_ZERO(&fd_in);
            FD_SET(fromTG, &fd_in);
            FD_SET(fdm, &fd_in);

            rc = select(std::max(fdm, fromTG) + 1, &fd_in, NULL, NULL, NULL);
            switch (rc)
            {
            case -1:
                fprintf(stderr, "Error %d on select()\n", errno);
                exit(1);

            default:
            {
                // If data on standard input
                if (FD_ISSET(fromTG, &fd_in))
                {
                    rc = read(fromTG, input, sizeof(input));
                    if (rc > 0)
                    {
                        // Send data on the master side of PTY
                        parent.logSL("data (fromTG) writing to fdm: ");
                        parent.logNS(input);
                        input[strlen(input)] = '\n';
                        write(fdm, input, rc);
                        write(1, input, rc);
                    }
                    else
                    {
                        if (rc < 0)
                        {
                            fprintf(stderr, "Error %d on read standard input\n", errno);
                            exit(1);
                        }
                    }
                }

                // If data on master side of PTY
                if (FD_ISSET(fdm, &fd_in))
                {
                    parent.logSL("data from the remote shell (pty master)");
                    char temp_input[50];
                    rc = read(fdm, temp_input, sizeof(temp_input));
                    if (rc > 0)
                    {
                        // Send data on standard output (toTG)
                        //parent.logNS(input);
                        //
                        //filtered_str = std::regex_replace(filtered_str, std::regex("?2004h"), " ");
                        //filtered_str = std::regex_replace(filtered_str, std::regex("^@"), " ");
                        //parent.log(filtered_str);
                        write(1, temp_input, rc);
                        //
                        /*char t[250];                
                        int i=0;            
                        while(temp_input[i]!='\0')
                        {
                            t[i] = temp_input[i];
                            i++;
                        }
                        parent.logSL("temp_input to toTG = ");
                        parent.logNS(t);*/
                        write(toTG, temp_input, rc);
                    }
                    else
                    {
                        if (rc < 0)
                        {
                            fprintf(stderr, "Error %d on read master PTY\n", errno);
                            exit(1);
                        }
                    }
                }
            }
            } // End switch
        }     // End while
    }
    else
    {
        struct termios slave_orig_term_settings; // Saved terminal settings
        struct termios new_term_settings;        // Current terminal settings

        // CHILD

        // Close the master side of the PTY
        close(fdm);

        // Save the defaults parameters of the slave side of the PTY
        rc = tcgetattr(fds, &slave_orig_term_settings);

        // Set RAW mode on slave side of PTY
        new_term_settings = slave_orig_term_settings;
        cfmakeraw(&new_term_settings);
        tcsetattr(fds, TCSANOW, &new_term_settings);

        // The slave side of the PTY becomes the standard input and outputs of the child process
        close(0); // Close standard input (current terminal)
        close(1); // Close standard output (current terminal)
        close(2); // Close standard error (current terminal)

        dup(fds); // PTY becomes standard input (0)
        dup(fds); // PTY becomes standard output (1)
        dup(fds); // PTY becomes standard error (2)

        // Now the original file descriptor is useless
        close(fds);

        // Make the current process a new session leader
        setsid();

        // As the child is a session leader, set the controlling terminal to be the slave side of the PTY
        // (Mandatory for programs like the shell to make them manage correctly their outputs)
        ioctl(0, TIOCSCTTY, 1);
        info.log("before execution of program");

        // Execution of the program
        {
            char **child_av;
            int i;

            // Build the command line
            child_av = (char **)malloc(ac * sizeof(char *));
            //std::string t("before loop,\tac = " + std::to_string(ac) + "\tfollowing cmds are passed on:");
            //info.log(t);
            for (i = 1; i < ac; i++)
            {
                child_av[i - 1] = strdup(av[i]);
                //info.log(av[i]);
            }
            child_av[i - 1] = NULL;

            //std::string t3(child_av[0]);
            //std::string t2("executing cmd : " + t3);
            //info.log(t2);
            rc = execvp(child_av[0], child_av);
            //info.log("executed");
        }

        // if Error...
        return 1;
    }

    return 0;
} // main