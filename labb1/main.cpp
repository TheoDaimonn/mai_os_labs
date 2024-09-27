#include "include/parent.hpp"


int main(void) {
    // export PATH_TO_CHILD1="/home/ivzarru/mai_os_labs/mai_os_labs/lab1/build/child1"
    // export PATH_TO_CHILD2="/home/ivzarru/mai_os_labs/mai_os_labs/lab1/build/child2"

    ParentProcess(getenv("PATH_TO_CHILD1"), getenv("PATH_TO_CHILD2"), std::cin, std::cout);

    exit(EXIT_SUCCESS);
}