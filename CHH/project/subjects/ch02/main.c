#include "file_manager.h"

int main(int argc, char** argv) {

    if(argc < 2){
        fprintf(stderr, "Argument less than two \n");
        return -1;
    }

    files_open(argc, argv);
    

    files_close();

    return 1;
}
