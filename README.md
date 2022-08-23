# carrie_cotg

Proof of concept of the project Code-On-The-Go (COTG)

## Installation Process

1. Ensure that your system has the following installed on your system:
  - OpenSSL
  - Boost

2.run the following commands:
  - mkdir build
  - mkdir build/logs
  - mkdir pipes
  - mkfifo pipes/fromTG.fifo -m0666
  - mkfifo pipes/toTG.fifo -m0666
  - mkfifo pipes/pass_chatId.fifo -m0666
  - cd build
  - cmake ../
    
3.Build the 3 targets required for the application to run : carrie, fromTG, toTG (in the build folder : cmake --build . --target carrie fromTG toTG)

4.Before running these executables, set the TERM variable to dumb to remove excess characters printed during the output.

6.Start 3 seperate terminals and all the above 3 programs.

---
## Modifying application : 

currently the fromTG and toTG have the token for the telegram bot hardcoded. Incase if you're creating a new telegram bot to test this on, update the token key and rebuild the applications. In successive versions, this method needs to be replaced with better ones.
