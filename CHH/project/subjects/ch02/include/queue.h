#ifndef D_QUEUE_H
#define D_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>

struct fd_queue{
    struct fd_queue* next;
    int fd;
};

struct data_queue{
    struct data_queue* next;
    int data;
    int freq;
};

typedef struct fd_queue fd_queue_t;
typedef struct data_queue data_queue_t;

void fd_enqueue(fd_queue_t* front, int fd);
int fd_dequeue(fd_queue_t* front);
void data_enqueue(data_queue_t* front, int data);
int data_dequeue(data_queue_t* front);

void print_queue(data_queue_t* front);

#endif /* D_QUEUE_H */ 