# carrie_cotg

Proof of concept of the project Code-On-The-Go (COTG)

## Installation Process

1. [Pre-Requisites] : Ensure that your system has the following installed on your system:
  - OpenSSL
  - Boost

2. [Setup] : run the following commands in sequence:
```
  - mkdir build
  - mkdir build/logs
  - mkdir pipes
  - mkfifo pipes/fromTG.fifo -m0666
  - mkfifo pipes/toTG.fifo -m0666
  - mkfifo pipes/pass_chatId.fifo -m0666
  - cd build
  - cmake ../
```
    
3. [Build] : Build the following 3 targets : carrie, fromTG, toTG 
in the build folder run: 
```
cmake --build . --target carrie fromTG toTG
```

4. [Run] : Open three separate terminals and in the build folder, run the following applications:
```
./fromTG
```
```
./carrie sh
```
```
./toTG
```

---
## Modifying application : 

currently the fromTG and toTG have the token for the telegram bot hardcoded. 
If you intend on creating a new telegram bot to work with, create a new telegram bot and obtain the token key. Update this in the program and rebuild the application. 
In successive versions, this method needs to be replaced with better ones.

---
## Future-Work:

This project has many opportunities to improve on, including:
<ul>
  <li> Do a market research survey to identify need and price point.
  <li> Extend this work to support Discord.
  <li> Include support for text-color and application specific commands (eg. clear/cls clears the telegram window instead of the remote terminal)
  <li> Ability to transfer file to the remote system within the chat-interface.
  <li> Add security measures like MFA and SUDO-commands monitoring to prevent accidental damage or unauthorized usage.
  <li> Support for Escape-Sequences (Edit/modify existing response-text to animate a progress bar)
  <li> Command black/white-listing (Regex) and directory compartmentalization.
</ul>

## Contribution:
If you're interested in contributing to this project, feel free to reach me out at shah.manthan38@gmail.com
