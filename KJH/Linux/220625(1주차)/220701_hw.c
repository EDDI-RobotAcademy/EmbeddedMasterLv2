#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define BUDDY_PAGE_SIZE						4096
#define SLAB_CACHE							64
#define ERROR								-1

int main(void)
{
	int fd, buf_len, search_word_len, change_word_len, strcmp_result, buf_offset_cursor = 0;
    int find_string_loaction_cursor = 0;
    int change_offset = 0;
	ssize_t nwrite, nread;

    int origin_string_loaction_buf[BUDDY_PAGE_SIZE] = {0,};
    int find_string_loaction_buf[BUDDY_PAGE_SIZE] = {0,};
	char origin_buf[BUDDY_PAGE_SIZE] = "apple1 test2 apple3 test4 apple5 test6 apple7 test8 apple9 test10 apple11 test12 apple13 test14 apple15 test16 apple17 test18\n";
    char search_word_buf[BUDDY_PAGE_SIZE] = "";
    char change_word_buf[BUDDY_PAGE_SIZE] = "";
    char new_buf[BUDDY_PAGE_SIZE] = "";

	fd = open("220701_hw.txt", O_RDWR | O_CREAT /*| O_TRUNC*/, 0644);

	if (fd == ERROR)
	{
		exit(1);
	}

	buf_len = strlen(origin_buf);
    printf("strlen(origin_buf) : %d\n", buf_len);
	nwrite = write(fd, origin_buf, buf_len);

	if (nwrite != buf_len)
	{
		printf("write error!\n");
		exit(2);
	}

    /////

    nread = read(fd, origin_buf, sizeof(origin_buf));
    printf("%s\n", origin_buf);

    strcpy(new_buf, origin_buf);
	
    buf_len = strlen(origin_buf);

    printf("찾을 단어를 입력해 주세요 : ");
    scanf("%s", search_word_buf);
    search_word_len = strlen(search_word_buf);
    //printf("입력한 Find word : %s\n", search_word_buf);

    printf("바꿀 단어를 입력해 주세요 : ");
    scanf("%s", change_word_buf);
    change_word_len = strlen(change_word_buf);
    //printf("입력한 Search word : %s\n", search_word_buf);


    while(buf_offset_cursor + search_word_len < buf_len)
    {
        strcmp_result = strncmp(origin_buf + buf_offset_cursor, search_word_buf, search_word_len);
        //printf("strcmp_result : %d\n", strcmp_result);
        if(strcmp_result == 0)
        {
            //printf("buf_offset_cursor : %d\n", buf_offset_cursor);
            origin_string_loaction_buf[find_string_loaction_cursor] = buf_offset_cursor;
            printf("%s\n", new_buf);
            find_string_loaction_cursor++;
        }  
        buf_offset_cursor++;
    }
    

    for(int i = 0 ; i < find_string_loaction_cursor ; i++)
    {
        printf("%d :: %d\n", i, origin_string_loaction_buf[i]);
    }
    buf_offset_cursor = 0;
    find_string_loaction_cursor = 0;



    while(buf_offset_cursor + search_word_len < buf_len)
    {
        strcmp_result = strncmp(new_buf + buf_offset_cursor, search_word_buf, search_word_len);
        //printf("strcmp_result : %d\n", strcmp_result);
        if(strcmp_result == 0)
        {
            //printf("buf_offset_cursor : %d\n", buf_offset_cursor);
            find_string_loaction_buf[find_string_loaction_cursor] = buf_offset_cursor;

            strcpy(new_buf + find_string_loaction_buf[find_string_loaction_cursor], change_word_buf);
            //printf("%s\n", new_buf);
            //strcpy(new_buf + find_string_loaction_buf[find_string_loaction_cursor] + change_word_len, origin_buf + origin_string_loaction_buf[find_string_loaction_cursor] + search_word_len * (find_string_loaction_cursor+1) );
            strcpy(new_buf + find_string_loaction_buf[find_string_loaction_cursor] + change_word_len, origin_buf + origin_string_loaction_buf[find_string_loaction_cursor] + search_word_len/* * (find_string_loaction_cursor+1)*/ );
            // printf("%s\n", origin_buf);
            // printf("%d, %d, %d, %d\n", origin_string_loaction_buf[find_string_loaction_cursor] + search_word_len * (find_string_loaction_cursor+1), origin_string_loaction_buf[find_string_loaction_cursor], search_word_len * (find_string_loaction_cursor+1), (find_string_loaction_cursor+1));
            // printf("%s\n", origin_buf + (origin_string_loaction_buf[find_string_loaction_cursor] + search_word_len * (find_string_loaction_cursor+1)));
            // printf("%s\n", new_buf);
            printf("==status==\nfind_string_loaction_cursor : %d\nchange_word_len : %d\nbuf_offset_cursor : %d\n\n", find_string_loaction_cursor, change_word_len, buf_offset_cursor);
            find_string_loaction_cursor++;

            buf_len = strlen(new_buf);
        }  
        buf_offset_cursor++;
    }

    for(int i = 0 ; i < find_string_loaction_cursor ; i++)
    {
        printf("%d 번째 위치 %d\n", i+1, find_string_loaction_buf[i]);
    }

    change_offset = change_word_len - search_word_len;
    
    strcpy(new_buf, origin_buf);

    for(int i = 0 ; i < find_string_loaction_cursor ; i++)
    {
        // strcpy(new_buf + find_string_loaction_buf[i], search_word_buf);
        // strcpy(new_buf + find_string_loaction_buf[i] + change_word_len, origin_buf + find_string_loaction_buf[i] + search_word_len);
        // printf("%s\n", new_buf);
    }

    //strcpy(new_buf + find_string_loaction_buf[0] + change_word_len, origin_buf + find_string_loaction_buf[0] + search_word_len);
    //strcpy(new_buf + find_string_loaction_buf[0], search_word_buf);
    // 0보다 큰경우

    // origin_buf 보다 작은 경우
    
    
    //printf("%s", new_buf);

    nwrite = write(fd, new_buf, buf_len);

	if (nwrite != buf_len)
	{
		printf("write error!\n");
		exit(2);
	}

	close(fd);

    printf("숙제끝!");

	return 0;
}
