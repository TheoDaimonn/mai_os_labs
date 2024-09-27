#include <utils.hpp>


pid_t CreateChild(){
    if(pid_t pid = fork(); pid >= 0) {
        return pid;
    }
    std::perror("Couldn't create child.");
    exit(EXIT_FAILURE);   
}


void CreatePipe(int pipeFd[2]){
    if( pipe(pipeFd) < 0 ) {
        std::perror("Couldn't create pipe.");
        exit(EXIT_FAILURE);
    }
}

void Exec(const char * pathToChild){
    if (execl(pathToChild, pathToChild, nullptr) == -1) {
        perror("Failed to exec.");
        exit(EXIT_FAILURE);
    }
}


std::stringstream ReadFromPipe (int file_descriptor){   
    std::stringstream stream;

    while (1){
        unsigned char buf[256] = "";
        ssize_t state = read(file_descriptor, &buf, 256UL);
        if (state > 0){
            stream << buf;
        }else if (state == 0){
            return stream;
        }else{
            perror("Read fail");
            exit(EXIT_FAILURE);
        }
    }

}