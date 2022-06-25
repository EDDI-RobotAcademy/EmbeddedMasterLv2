#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _Queue_t{
    char data;
    struct _Queue_t* link;
 } Queue_t;

Queue_t* Queue_create(void)
{
   Queue_t *p_head;

   p_head = (Queue_t*)malloc(sizeof(Queue_t));
   p_head->link = NULL;
   p_head->data = (char)0;

   return p_head;
}

bool Queue_empty(Queue_t i_p_head)
{
   if(i_p_head = NULL) 
   {

   }
}

void Queue_closeAll(Queue_t* i_p_head)
{
    int n_iterator;

    while(i_p_head != NULL) {
        free(i_p_head);
        i_p_head = i_p_head->link;
    }
}

void Queue_printfAll(Queue_t *i_p_head)
{
    int n_iterator = 0;

    while(i_p_head != NULL) {
        printf("Queue Data[%d] = %d\n\r",n_iterator++, i_p_head->data);
        i_p_head = i_p_head->link;
    }
}

char Queue_get(Queue_t** i_p_head)
{
    char r_data;
    Queue_t *p_tmp_head;

    if(*i_p_head != NULL)
    {
        p_tmp_head = *i_p_head;
        r_data = (*i_p_head)->data;
        *i_p_head = (*i_p_head)->link;
        free(p_tmp_head);
    }

    return r_data; 
}

void Queue_set(Queue_t** i_p_head, char* i_p_data) 
{
    if( *i_p_head == NULL ) {
        *i_p_head = Queue_create();
        (*i_p_head)->data = *i_p_data;
        return;
    } 

    Queue_set(&((*i_p_head)->link), i_p_data);
}

int main(int argc, char* argv[])
{
    Queue_t* p_head = NULL;
    char p_data[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    char data;
    int n_iterator;

    for( n_iterator = 0; n_iterator < sizeof(p_data); n_iterator++ ) {
        Queue_set(&p_head, &p_data[n_iterator]);
    }

    Queue_printfAll(p_head);

    data = Queue_get(&p_head);
    printf("Dequeue Data = %d\r\n", data);

    Queue_printfAll(p_head);

    Queue_closeAll(p_head);

    return 0;
}