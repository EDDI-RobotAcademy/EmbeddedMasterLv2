#if 0
### TODO ### 
#1. 두 개의 파일 test1.txt test2.txt로 부터 데이터를 읽는다.
#2. 1개의 버퍼를 만들어 내용을 다 때려 넣는다.
#3. Queue(list 형태)를 동적할당으로 하나씩 생성
#4. Queue의 내용은 freq(빈도수), data 조합의 구조체로 구성한다.
#5  구현 중에 freq 는 같은 숫자의 갯수를 의미하며 Queue에 data를 쌓아갈 때 같은 data가 들어오면 freq를 +1씩 증가하여 입력
#6. data가 다르면 리스트 형태로 queue 새로 생성하여 뒤에 이어 붙인다.
#7. 출력시 freq 내용을 확인 가능하도록 구현

### 주의사항 ### 
*1. 두 개의 파일로부터의 data는 문자열로 저장되기 때문에 이를 어떻게 숫자로 구별하고 버퍼에 저장할지 고민하기
*1_1. strtok 과 같은 함수로 delim 인자를 '\n'로 띄어쓰기 기준으로 단여를 구별
*1_2. 구별된 단어를 atoi 함수를 이용하여 숫자로 변경하여 숫자를 저장하는 버퍼에 저장
*2. Queue_set 함수에 구현된 내용을 freq가 적용되도록 약간 수정 필요
#endif

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

//#4. Queue의 내용은 freq(빈도수), data 조합의 구조체로 구성한다.
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

// #2. Queue_set 함수에 구현된 내용을 freq가 적용되도록 약간 수정 필요
void Queue_set(Queue_t** i_p_head, int* i_p_data) 
{
    // #6. data가 다르면 리스트 형태로 queue 새로 생성하여 뒤에 이어 붙인다.
    if( *i_p_head == NULL ) {
        *i_p_head = Queue_create();
        (*i_p_head)->data = *i_p_data;
        return;
    } 
    // #5  구현 중에 freq 는 같은 숫자의 갯수를 의미하며 Queue에 data를 쌓아갈 때 같은 data가 들어오면 freq를 +1씩 증가하여 입력
    else if( ((*i_p_head)->data) == *i_p_data ) {
        (*i_p_head)->freq += 1;
        return;
    }

    Queue_set(&((*i_p_head)->link), i_p_data);
}

// #7. 출력시 freq 내용을 확인 가능하도록 구현
void Queue_printfAll(Queue_t *i_p_head)
{
    int iterator = 0;

    while(i_p_head != NULL) {
        printf("Queue[%d] : freq = %d\tData = %d\n",iterator, i_p_head->freq, i_p_head->data);
        printf("--------------------------------\n");
        i_p_head = i_p_head->link;
        iterator++;
    }
}

void Queue_get(Queue_t** i_p_head, int* o_p_data, int* o_p_freq)
{
    int rtn_data;
    int rtn_freq;
    Queue_t *p_tmp_head;

    if(*i_p_head != NULL)
    {
        p_tmp_head = *i_p_head;
        rtn_data = (*i_p_head)->data;
        rtn_freq = (*i_p_head)->freq;
        *i_p_head = (*i_p_head)->link;
        free(p_tmp_head);
    }

    *o_p_data = rtn_data;
    *o_p_freq = rtn_freq;
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

    //#1. 두 개의 파일 test1.txt test2.txt로 부터 데이터를 읽는다.
    fd1 = open("test1.txt", O_RDONLY, 0644);
    fd2 = open("test2.txt", O_RDONLY, 0644);

    if( (fd1 == ERROR || fd2 == ERROR) ) {
        perror("open error\n");
        exit(1);
    }

    //#2. 1개의 버퍼를 만들어 내용을 다 때려 넣는다.
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


    // *1. 두 개의 파일로부터의 data는 문자열로 저장되기 때문에 이를 어떻게 숫자로 구별하고 버퍼에 저장할지 고민하기
    // *1_1. strtok 과 같은 함수로 delim 인자를 '\n'로 띄어쓰기 기준으로 단여를 구별
    // *1_2. 구별된 단어를 atoi 함수를 이용하여 숫자로 변경하여 숫자를 저장하는 버퍼에 저장
    for (tok = strtok(buf, delim); tok != NULL; tok = strtok(NULL, delim))
    {
        // printf("%s\n",tok);
        // strcpy(ptr[cnt++], tok);
        ptr[cnt] = (char *) tok;
        printf("ptr[%d] = %s\n", cnt, ptr[cnt]);

        data_buf[cnt] = atoi(ptr[cnt]);

        cnt++;
    }

    //#3. Queue(list 형태)를 동적할당으로 하나씩 생성
    for( n_iterator = 0; n_iterator < cnt; n_iterator++ ) {
        Queue_set(&p_head, &data_buf[n_iterator]);
    }

    Queue_printfAll(p_head);
    return 0;
}
