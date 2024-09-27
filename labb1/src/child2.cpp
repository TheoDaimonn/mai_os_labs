#include <string>
#include <iostream>
#include <algorithm>

int main(){
    std::string line;
    while(std::getline(std::cin, line)){
        std::string new_line = "";
        for (auto i:line){
            if (i == ' '){
                new_line += "_";
            }
            else{
                new_line += i;
            }
        }
        
        std::cout << new_line << std::endl;   
    }
    exit(EXIT_SUCCESS);
}