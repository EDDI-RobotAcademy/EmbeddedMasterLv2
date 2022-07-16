#include "queue.h"

void fd_enqueue(fd_queue_t* front, int fd)
{
    fd_queue_t* tmp_ptr = front;

    while(tmp_ptr != NULL)
    {
        tmp_ptr = tmp_ptr->next;
    }

    tmp_ptr = (fd_queue_t*)malloc(sizeof(fd_queue_t));
    tmp_ptr->fd = fd;
    tmp_ptr->next = NULL;
    return;
}

int fd_dequeue(fd_queue_t* front)
{
    fd_queue_t* delete_node;
    int ret_int;

    if(front == NULL) return -1;

    delete_node = front;
    ret_int = front->fd;
    front = front->next;

    free(delete_node);
    return ret_int;
}

void data_enqueue(data_queue_t* front, int data)
{
    data_queue_t* tmp_ptr = front;

    if (tmp_ptr == NULL)
    {
        tmp_ptr = (data_queue_t*)malloc(sizeof(data_queue_t));
        tmp_ptr->data = data;
        tmp_ptr->next = NULL;
        tmp_ptr->freq = 1;
    }

    while(tmp_ptr != NULL)
    {
        if(tmp_ptr->data == data)
        {
            tmp_ptr->freq += 1;
            break;
        }
        tmp_ptr = tmp_ptr->next;
    }

    tmp_ptr = (data_queue_t*)malloc(sizeof(data_queue_t));
    tmp_ptr->data = data;
    tmp_ptr->next = NULL;
    tmp_ptr->freq = 1;

}

int data_dequeue(data_queue_t* front)
{
    data_queue_t* delete_node;
    int ret_int;

    if(front == NULL) return -1;

    delete_node = front;
    ret_int = front->data;
    front = front->next;

    free(delete_node);
    return ret_int;
}

void print_queue(data_queue_t* front)
{
    data_queue_t* tmp_ptr = front;

    while(tmp_ptr != NULL)
    {
        printf("data : %08d,   freq : %08d \n", tmp_ptr->data, tmp_ptr->freq);
    }
}