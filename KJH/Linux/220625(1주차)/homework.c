#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "homework.h"

int main(void)
{
	int fd, buf_len, strcmp_result;
    
    int search_word_len, change_word_len;
    
	ssize_t nwrite, nread;
	
    char origin_buf[BUDDY_PAGE_SIZE] = "";
    char new_buf[BUDDY_PAGE_SIZE] = "";

    char search_word_buf[BUDDY_PAGE_SIZE] = "";
    char change_word_buf[BUDDY_PAGE_SIZE] = "";
    int buf_offset_cursor = 0;
    
    int origin_string_loaction_buf[BUDDY_PAGE_SIZE] = {0,};
    int find_string_loaction_buf[BUDDY_PAGE_SIZE] = {0,};
    int find_string_loaction_cursor = 0;


	fd = open("article.txt", O_RDWR | O_CREAT /*| O_TRUNC*/, 0644);

	if (fd == ERROR)
	{
		exit(1);
	}


    nread = read(fd, origin_buf, sizeof(origin_buf));
    buf_len = strlen(origin_buf);
    printf("%s\n", origin_buf);


    strcpy(new_buf, origin_buf);


    printf("찾을 단어를 입력해 주세요 : ");
    scanf("%s", search_word_buf);
    search_word_len = strlen(search_word_buf);

    printf("바꿀 단어를 입력해 주세요 : ");
    scanf("%s", change_word_buf);
    change_word_len = strlen(change_word_buf);


    // 찾을 단어 위치를 찾습니다. --- (1)
    while(buf_offset_cursor + search_word_len < buf_len)
    {
        strcmp_result = strncmp(origin_buf + buf_offset_cursor, search_word_buf, search_word_len);
        if(strcmp_result == 0)
        {
            origin_string_loaction_buf[find_string_loaction_cursor] = buf_offset_cursor;
            find_string_loaction_cursor++;
        }  
        buf_offset_cursor++;
    }
    buf_offset_cursor = 0;
    find_string_loaction_cursor = 0;


    // (1)에서 찾은 위치를 이용해 바꿀 단어를 넣어줍니다.
    while(buf_offset_cursor + search_word_len < buf_len)
    {
        strcmp_result = strncmp(new_buf + buf_offset_cursor, search_word_buf, search_word_len);

        if(strcmp_result == 0)
        {
            find_string_loaction_buf[find_string_loaction_cursor] = buf_offset_cursor;

            strcpy(new_buf + find_string_loaction_buf[find_string_loaction_cursor], change_word_buf);   // 바꿀 위치에 바꿀 단어를 복사하여 넣어줍니다.
            strcpy(new_buf + find_string_loaction_buf[find_string_loaction_cursor] + change_word_len,   // 'origin_buf'에서 바뀐 단어의 수를 계산해 뒷 내용이 들어갈 포인터 위치를 결정합니다.
                    origin_buf + origin_string_loaction_buf[find_string_loaction_cursor] + search_word_len ); 
            find_string_loaction_cursor++;

            buf_len = strlen(new_buf);
        }  
        buf_offset_cursor++;
    }
    close(fd);

    //기존 파일 내용을 삭제하기 위해 WRONLY로 
    fd = open("article.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == ERROR)
	{
		exit(1);
	}
    
    lseek(fd, (off_t)0, SEEK_SET);          // 파일 읽기/쓰기 포인터의 위치를 0으로 초기화 시켜줍니다.
    nwrite = write(fd, new_buf, buf_len);   // 바뀐 내용을 파일에 씁니다.

	if (nwrite != buf_len)
	{
		printf("write error!\n");
		exit(2);
	}

	close(fd);

    printf("%d 단어 변환 완료 숙제끝 !\n", find_string_loaction_cursor);

	return 0;
}
