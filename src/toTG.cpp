#include "fifo.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fstream>

#include <libtelegram/libtelegram.h>
#define MSG_COUNT_MAX 30

void dbg(const char *dbgmsg) { std::cout << "[debug] : " << dbgmsg; }
bool is_utf8(const char *str);

char* process_codes(char* inp);

int odd = 0;
int flag = 0;


auto main() -> int
{
    std::string const token("5226577382:AAFLgB7yp4v69UuX9HtReM1T_0W7ivikPrU"); // in practice you would probably read this from config file or database
    telegram::sender sender(token);                                            // create a sender with our token for outgoing messages
    telegram::listener::poll listener(sender);
    char buf[BUF_SIZE] = {0};
    int rc = 0;
    int64_t my_chat_id = 0;
    int msg_count = 0;

    //create a logfile for later analysis
    std::ofstream log;
    log.open("logs/toTG_logs.txt");
    log << "[begin]" << std::endl;

    int test_fifo = open("../pipes/pass_chatId.fifo", O_RDONLY);
    if (test_fifo < 1)
    {
        std::cout << "failed to open test_fifo";
        exit(-1);
    }

    do
    {
        rc = read(test_fifo, buf, BUF_SIZE);
    } while (!(rc > 0));

    my_chat_id = atol(buf);
    std::cout << "chat id = " << buf;
    std::string const respond("chat id received");
    sender.send_message(my_chat_id, respond);
    //close(test_fifo);

    int toTG = open("../pipes/toTG.fifo", O_RDONLY);
    if (toTG < 1)
    {
        std::cout << "failed to open toTG";
        exit(-1);
    }
    else
    {
        std::cout << "\n\ntoTG fifo opened\n\n";
    }

    while(1)
    {
        rc = read(toTG, buf, BUF_SIZE);
        std::cout<< ".";
        char buf2[BUF_SIZE] = {0};
        if (rc > 0)
        {
            printf("\n[debug] : received text : %s\n",buf);
            int j=0;
            for(int i=0;i<250;i++)
            {
                if(buf[i] =='^' && buf[i+1]=='@')
                {
                    i+=3;
                }else
                {
                    buf2[j++] = buf[i];
                }
                if(buf[i] == '%')
                {
                    flag = 1;
                }

                if(buf[i] == '\n' && flag == 1)
                {
                    break;
                }
                flag = 0;
            }
            std::string temp(buf2);
            // write to log file for later analysis
            log << temp << std::endl;
            sender.send_message(my_chat_id, temp);
            for(int i=0;i<BUF_SIZE;i++) buf[i] = 0;

        }
    }
    log.close();
    exit(0);
}

char* process_codes(char* inp)
{
    char esc_code_start = "[?2004l";
    char esc_code_end = "[?2004h";
    return 
}