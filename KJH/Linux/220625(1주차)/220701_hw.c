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
	int fd, buf_len, buf_len1, buf_len2, strcmp_result, buf_offset = 0;
    int find_string_loaction_cursor = 0;
    int change_offset = 0;
	ssize_t nwrite, nread;

    int find_string_loaction_buf[BUDDY_PAGE_SIZE] = {0,};
	char buf[BUDDY_PAGE_SIZE] = "apple test apple test apple test apple test apple test apple test apple test apple test apple test\n";
    char input_buf[BUDDY_PAGE_SIZE] = "";
    char new_buf[BUDDY_PAGE_SIZE] = "";

	fd = open("220701_hw.txt", O_WRONLY | O_CREAT /*| O_TRUNC*/, 0644);

	if (fd == ERROR)
	{
		exit(1);
	}

	buf_len = strlen(buf);
    printf("strlen(buf) : %d\n", buf_len);
	nwrite = write(fd, buf, buf_len);

	if (nwrite != buf_len)
	{
		printf("write error!\n");
		exit(2);
	}

    /////

    nread = read(fd, buf, sizeof(buf));
	printf("%s\n", buf);

    buf_len = strlen(buf);

    printf("찾을 단어를 입력해 주세요 : ");
    scanf("%s", input_buf);
    buf_len1 = strlen(input_buf);

    //printf("입력한 Find word : %s\n", input_buf);

    while(buf_offset+buf_len1 < buf_len)
    {
        strcmp_result = strncmp(buf+buf_offset, input_buf, buf_len1);
        //printf("strcmp_result : %d\n", strcmp_result);
        if(strcmp_result == 0)
        {
            //printf("buf_offset : %d\n", buf_offset);
            find_string_loaction_buf[find_string_loaction_cursor++] = buf_offset;
        }  
        buf_offset++;
    }

    for(int i = 0 ; i < find_string_loaction_cursor ; i++)
    {
        printf("%d 번째 위치 %d\n", i+1, find_string_loaction_buf[i]);
    }

    printf("바꿀 단어를 입력해 주세요 : ");
    scanf("%s", input_buf);
    buf_len2 = strlen(input_buf);
    
    //printf("입력한 Search word : %s\n", input_buf);

    change_offset = buf_len2 - buf_len1;
    
    strcpy(new_buf, buf);
    strcpy(new_buf + find_string_loaction_buf[0], input_buf);
    strcpy(new_buf + find_string_loaction_buf[0] + buf_len2, buf + find_string_loaction_buf[0] + buf_len1);
    printf("%s\n", new_buf);
    //strcpy(new_buf + find_string_loaction_buf[0] + buf_len2, buf + find_string_loaction_buf[0] + buf_len1);
    //strcpy(new_buf + find_string_loaction_buf[0], input_buf);
    // 0보다 큰경우

    // buf 보다 작은 경우
    
    
    //printf("%s", new_buf);

	close(fd);

	return 0;
}
