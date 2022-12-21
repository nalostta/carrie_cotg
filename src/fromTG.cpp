#include "fifo.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>

#include <libtelegram/libtelegram.h>

#include "logger.hpp"

const int MSG_BUF_LEN=250;

int fromTG;
char temp[MSG_BUF_LEN];

int flag = 1;
auto main() -> int
{
    logger info("logs/fromTG.info.log");
    info.log("start");
    fromTG = open("../pipes/fromTG.fifo", O_WRONLY);
    if (fromTG < 1)
    {
        std::cout << "failed to open fromTG.fifo";
        exit(-1);
    }

    /*---to pass chat id----*/
    int test_fifo = open("../pipes/pass_chatId.fifo", O_WRONLY);
    if (test_fifo < 1)
    {
        std::cout << "failed to open pass_chatID.fifo";
        exit(-1);
    }

    char buf[BUF_SIZE];
    int rc = 0;

    std::string const token("5226577382:AAFLgB7yp4v69UuX9HtReM1T_0W7ivikPrU");  // in practice you would probably read this from config file or database                                          
    telegram::sender sender(token);                                             // create a sender with our token for outgoing messages
    telegram::listener::poll listener(sender);                                  // create a polling listener which will process incoming requests, polling using the sender
    info.log("created telegram listener");
    listener.set_callback_message([&](telegram::types::message const &message) {
        if (flag)
        {
            info.log("flag triggered,setting to 0");

            std::cout<< "writing chat id : " << message.chat.id;
            std::string temp = std::to_string(message.chat.id);
            write(test_fifo,temp.c_str(),temp.length());
            close(test_fifo);
            flag = 0;
        }
        if (message.text == "/stop")
        {
            printf("\n\n\n---stopping service");
            info.log("/stop received");
            listener.stop();
            info.log("listener stopped");
            close(fromTG);
        }
            strcpy(temp,message.text->c_str());
            std::cout<< "\n[debug] : " << temp;
            info.logSL("received message :");
            info.logNS(*message.text);
            write(fromTG, temp, sizeof(temp));
            memset(temp,0,MSG_BUF_LEN);
        });
        listener.run();
        return EXIT_SUCCESS;
}