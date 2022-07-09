#include "file_manager.h"
#include "queue.h"

static fd_queue_t* g_files = NULL;
static data_queue_t* g_data = NULL;

bool files_open(int argc, char** argv)
{
    u_int32_t i;
    int fd;

    for(i =  1; i < argc; i++)
    {
        fd = open(argv[i], O_RDONLY);
        if(fd == -1) {
            fprintf(stderr, "file open failed !!\n");
            return false;
        }
        printf("file open : %s \n", argv[i]);
        fd_enqueue(g_files, fd);
    }

    return true;
}

void files_read()
{


}

void files_close()
{
    int fd;
    int data;

    fd = fd_dequeue(g_files);
    data = data_dequeue(g_data);

    while(fd)
    {
        close(fd);
        fd = fd_dequeue(g_files);        
    }

    while(data)
    {
        data = data_dequeue(g_data);
    }

}