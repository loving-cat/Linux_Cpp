#include "namedPipe.hpp"

//write
int main()
{
    NamePiped fifo(comm_path,User);
    if(fifo.OpenForWrite())
    {
        while(true)
        {
            std::cout<<"Please Enter>";
            std::string message;
            std::getline(std::cin,message);
            fifo.WriteNamedPipe(message);  
        }        
    }
    return 0;
}