#include <parent.hpp>


void ParentProcess(const char * pathToChild1, const char * pathToChild2, std::istream & streamIn, std::ostream & streamOut ){
    int child1In[2];
    CreatePipe(child1In);

    int child2Out[2];
    CreatePipe(child2Out);

    int pipeBetween[2];
    CreatePipe(pipeBetween);


    pid_t pid = CreateChild(), pid2 = -1;
    if (pid == pid_t(0)){
        pid2 = CreateChild();
    }

    if (pid2 == pid_t(0)){
        close(child2Out[WRITE_END]);
        close(child1In[READ_END]);
        close(child1In[WRITE_END]);
        close(pipeBetween[READ_END]);
      
        if (dup2(child2Out[READ_END], STDIN_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }

        if (dup2(pipeBetween[WRITE_END], STDOUT_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }   
        Exec(pathToChild1);
    }
    else if (pid2 > pid_t(0)){
        close(child2Out[WRITE_END]);
        close(child2Out[READ_END]);
        close(pipeBetween[WRITE_END]);
        close(child1In[READ_END]);
      
        if (dup2(pipeBetween[READ_END], STDIN_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }

        if (dup2(child1In[WRITE_END], STDOUT_FILENO) == -1){
            perror("dup2 error");
            exit(EXIT_FAILURE);
        }   
        Exec(pathToChild2);
    }
    else{
        close(child1In[WRITE_END]);
        close(child2Out[READ_END]);
        close(pipeBetween[WRITE_END]);
        close(pipeBetween[READ_END]);

        std::string line;
        while(std::getline(streamIn, line)){
            line += "\n";
            write(child2Out[WRITE_END], line.c_str(), line.size());
        }
        close(child2Out[WRITE_END]);


        std::stringstream output = ReadFromPipe(child1In[READ_END]);

        while(std::getline(output, line)){
            streamOut << line << std::endl;
            
        }

        close(child1In[READ_END]);
    }
}