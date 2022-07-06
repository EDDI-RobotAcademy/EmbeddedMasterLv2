#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#define ERROR               -1
#define SLAB_SIZE		    64
#define BUDDY_PAGE_SIZE		4096

typedef struct _Queue_t{
    int freq;
    int data;
    struct _Queue_t* link;
 } Queue_t;

Queue_t* Queue_create(void)
{
   Queue_t *p_head;

   p_head = (Queue_t*)malloc(sizeof(Queue_t));
   p_head->link = NULL;
   p_head->data = 0;
   p_head->freq = 1;

   return p_head;
}

bool Queue_empty(Queue_t* i_p_head)
{
   if(i_p_head == NULL) 
   {
        return true;
   }
   return false;
}

void Queue_set(Queue_t** i_p_head, int* i_p_data) 
{
    if( *i_p_head == NULL ) {
        *i_p_head = Queue_create();
        (*i_p_head)->data = *i_p_data;
        return;
    } else if( ((*i_p_head)->data) == *i_p_data ) {
        (*i_p_head)->freq += 1;
        return;
    }

    Queue_set(&((*i_p_head)->link), i_p_data);
}

void Queue_printfAll(Queue_t *i_p_head)
{
    int n_iterator = 0;

    while(i_p_head != NULL) {
        printf("Queue[%d] : freq = %d\tData = %d\n",n_iterator, i_p_head->freq, i_p_head->data);
        printf("--------------------------------\n");
        i_p_head = i_p_head->link;
        n_iterator++;
    }
}

void Queue_get(Queue_t** i_p_head, int* o_p_data, int* o_p_freq)
{
    int r_data;
    int r_freq;
    Queue_t *p_tmp_head;

    if(*i_p_head != NULL)
    {
        p_tmp_head = *i_p_head;
        r_data = (*i_p_head)->data;
        r_freq = (*i_p_head)->freq;
        *i_p_head = (*i_p_head)->link;
        free(p_tmp_head);
    }

    *o_p_data = r_data;
    *o_p_freq = r_freq;
}

int main(int argc, char **argv)
{
    char buf[BUDDY_PAGE_SIZE];
    int data_buf[BUDDY_PAGE_SIZE];
    char* ptr[BUDDY_PAGE_SIZE];
    char* tok;
    char *delim = "\n";
    int buf_len;
    int fd1, fd2; 
    int rd_bytes1, rd_bytes2;
    int idx;
    int cnt = 0;
    int n_iterator;
    Queue_t* p_head = NULL;

    fd1 = open("test1.txt", O_RDONLY, 0644);
    fd2 = open("test2.txt", O_RDONLY, 0644);

    if( (fd1 == ERROR || fd2 == ERROR) ) {
        perror("open error\n");
        exit(1);
    }

    if( (rd_bytes1 = read(fd1, buf, sizeof(buf))) == ERROR ) {
        perror("read error\n");
        exit(1);
    }

    printf("rd_bytes1 = %d\n", rd_bytes1);
    printf("%s\n", buf);

    if ( (rd_bytes2 = read(fd2, &buf[rd_bytes1], sizeof(buf))) == ERROR ) {
        perror("read error\n");
        exit(1);
    } 

    printf("rd_bytes2 = %d\n", rd_bytes2);
    printf("%s\n", &buf[rd_bytes1]);

    for (tok = strtok(buf, delim); tok != NULL; tok = strtok(NULL, delim))
    {
        // printf("%s\n",tok);
        // strcpy(ptr[cnt++], tok);
        ptr[cnt] = (char *) tok;
        printf("ptr[%d] = %s\n", cnt, ptr[cnt]);

        data_buf[cnt] = atoi(ptr[cnt]);

        cnt++;
    }

    for( n_iterator = 0; n_iterator < cnt; n_iterator++ ) {
        Queue_set(&p_head, &data_buf[n_iterator]);
    }

    Queue_printfAll(p_head);
    return 0;
}