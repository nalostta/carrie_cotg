#include "fifo.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <libtelegram/libtelegram.h>

auto main() -> int
{
  int test_fifo = open("test_fifo", O_RDWR);
  if (test_fifo < 1)
  {
    std::cout << "failed to open fifo";
    exit(-1);
  }
  char buf[BUF_SIZE];
  int rc = 0;

  std::string const token("5226577382:AAFLgB7yp4v69UuX9HtReM1T_0W7ivikPrU"); // in practice you would probably read this from config file or database
  telegram::sender sender(token);                                            // create a sender with our token for outgoing messages
  telegram::listener::poll listener(sender);                                 // create a polling listener which will process incoming requests, polling using the sender
  listener.set_callback_message([&](telegram::types::message const &message) {
    write(test_fifo, message.text->c_str(), message.text->length()+1);
  });                                                
  listener.run();
}