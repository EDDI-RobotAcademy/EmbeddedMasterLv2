#ifndef D_FILIE_MANAGER_H
#define D_FILIE_MANAGER_H

#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

bool files_open(int argc, char** argv);
void files_close();

#endif /* D_FILIE_MANAGER_H */ 