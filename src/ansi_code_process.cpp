#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>

int main(int argc, char* argv[])
{
    std::ifstream log;
    assert(argc>1);
    log.open(argv[1]);
    if(log){std::cout<<"file opened successfully!\n";}else{std::cout<<"error opening log with the name "<<argv[1]<<std::endl;}
    std::vector<std::string> log_local;
    std::string line;
    while(log.good()){ log >> line; log_local.push_back(line);}
    log.close();
    //process line here
    //backspace is 08
    /*size_t posn = line.find(0x08); //backspace
    if(posn!=std::string::npos & posn!=0)
    {
        std::cout<<posn<<" \033[1;31m"<<line<<"\033[1;31m"<<std::endl;
        line.erase(posn-1,2);
    }*/
    //std::cout<<"\033[1;31m"<<line<<"\033[1;31m"<<std::endl;
    int logSize = log_local.size();
    for(int i=0; i<logSize; i++)
    {
        
        if(log_local[i][0]==0x08 && i>0)
        {
            auto it = std::next(log_local.begin(),i-1);
            log_local.erase(it);
        } 
        auto locn = log_local[i].find(0x1B);
        if(locn!=std::string::npos) //escape code found 
        {
            auto it = std::next(log_local.begin(),i);
            log_local.erase(log_local.begin()+i);
        } 
    }

    std::ofstream processed("./logs/toTG_processed.txt");
    int allGood = processed.good();
    assert(allGood);
    logSize = log_local.size();
    for(int i=0; i<logSize; i++)
    { 
        std::cout<<"\033[1;31m"<< log_local[i]<<"\033[1;31m"<<std::endl;
        processed<< log_local[i]<< std::endl;
    }
    processed.close();
}