#include <iostream>
#include "interactive_testing.h"
#include "file_testing.h"

int main(){
    std::cout<<"Testing modes:\n1. Interactive mode (stdin, stdout)\n2. File mode (input.txt to output.txt)"<<std::endl;
    std::cout<<"Enter choice."<<std::endl;
    int choice;
    std::cin>>choice;
    if(choice == 1) interactive_testing_mode();
    if(choice == 2) file_mode();
    return 0;
}
