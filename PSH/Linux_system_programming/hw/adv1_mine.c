#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define ERROR               -1
#define SLAB_SIZE		    64
#define BUDDY_PAGE_SIZE		4096

char src_buf[SLAB_SIZE] = "apple test apple test apple test apple test";
char cmp_buf[SLAB_SIZE] = { 0 };
char dst_buf[SLAB_SIZE] = { 0 };
char tmp_buf[SLAB_SIZE] = { 0 };

int src_len;
int cmp_len;
int dst_len;
int diff_len;

int fd_src;
int fd_dst;

off_t next_read_fpos = 0; 
off_t curr_read_fpos = 0; 

#if 0
### 사용법 ###

./a.out "apple" "watermelon" 
./a.out "apple" "go" 

 argv[1] = 바뀔 단어 
 argv[2] = 바꿀 단어 

### TODO ### 
#1. 파일 커서를 하나씩 증가시키며 read를 진행하며 "apple" 이라는 단어가 있는지 찾는다.
#2. 1번의 read는 찾을 파일인 "apple"의 크기 5만큼만 읽어서 strncmp로 비교한다.
#3. 다르다면 파일 커서를 하나씩 증가시키며 "apple" 찾기 반복
#4. 같다면 "watermelon"이 씌여지도록 dst_buf에 저장하고 "apple" 다음에 오는 문자열 "test apple test ..."은 tmp버퍼에 저장
#5. dst_buf에 tmp 버퍼 내용도 추가하여 하나의 문장 완성 후, 파일에 write함

### 주의사항 ### 
*1. 두 문자열의 크기가 서로 달라서 "watermelon"이 아닌 "go" 같이 더 짧은 경우도 대비  
*2. "apple" 이라는 단어를 찾아서 처리 후에는 파일 커서를 +1 해서 또 찾으면 불필요한 반복이 진행될 수 있음 
    apple 만큼 증가한 곳에 다음 파일 커서를 위치시키고 위의 과정을 반복한다.
    "apple test apple test apple test"
     ^    ^
     |    |
     0    5(커서)
###################################################
#endif

int main(int argc, char **argv)
{
    int write_bytes, read_bytes, read_remain_bytes;
    volatile int i, idx=0;

    src_len = strlen(src_buf);

    if( argc != 3) {
       printf("argument input error!\n"); 
       return -1;
    }

/* src_file.txt에 apple test ... write하고 read 테스트 */
    if( (fd_src = open("src_file.txt", O_RDWR | O_TRUNC | O_CREAT, 0644)) == ERROR ) {
        perror("open error!\n");
        exit(1);
    }

    if( (write_bytes = write(fd_src, src_buf, src_len)) == ERROR ) {
        perror("write error!\n");
        close(fd_src);
        exit(1);
    } else if( write_bytes != src_len ) {
        printf("didn't write all\n");
        close(fd_src);
        exit(1);
    }

    // printf("write_bytes : %d\n", write_bytes);
    lseek(fd_src, (off_t) 0, SEEK_SET);

    if( (read_bytes = read(fd_src, src_buf, write_bytes)) == ERROR ) {
        perror("read error!\n");
        close(fd_src);
        exit(1);
    } else if( read_bytes != write_bytes ) {
        printf("didn't read all\n");
        close(fd_src);
        exit(1);
    }

    // printf("read_bytes : %d\n", read_bytes);
    printf("src_buf : %s\n", src_buf);
    lseek(fd_src, (off_t) 0, SEEK_SET);

    cmp_len = strlen(argv[1]);
    dst_len = strlen(argv[2]);

    diff_len = dst_len - cmp_len;

/* file position을 증가해가며 바꿀 단어가 있는지 검색하고 변경하는 작업 진행 */
    for(i = 0; i < (src_len - cmp_len - (idx*cmp_len) + 1); i++)
    {
        // printf("cnt : %d\n", (src_len - cmp_len - (idx*cmp_len) + 1));
        lseek(fd_src, (off_t)next_read_fpos, SEEK_SET);

    /* 찾을 단어 크기만큼만 read해서 비교 진행*/
        // #1. 파일 커서를 하나씩 증가시키며 read를 진행하며 "apple" 이라는 단어가 있는지 찾는다.
        if( (read_bytes = read(fd_src, cmp_buf, cmp_len)) == ERROR ) {
            perror("read error!\n");
            close(fd_src);
            exit(1);
        }

        // #2. 1번의 read는 찾을 파일인 "apple"의 크기 5만큼만 읽어서 strncmp로 비교한다.
        // #4. 같다면 "watermelon"이 씌여지도록 dst_buf에 저장하고 "apple" 다음에 오는 문자열 "test apple test ..."은 tmp버퍼에 저장
        if( !(strncmp(cmp_buf, argv[1], cmp_len)) ) {
            // printf("dst_read_fpos_1 : %ld\n", next_read_fpos + ((idx) * diff_len));
            strncpy(&dst_buf[next_read_fpos + (idx * diff_len)], argv[2], dst_len);

            curr_read_fpos = lseek(fd_src, 0, SEEK_CUR);

            // printf("dst_read_fpos_2 : %ld\n", next_read_fpos + (idx * diff_len) + dst_len);

            // #5. dst_buf에 tmp 버퍼 내용도 추가하여 하나의 문장 완성 후, 파일에 write함
            read_remain_bytes = read(fd_src, tmp_buf, SLAB_SIZE);
            // *1. 두 문자열의 크기가 서로 달라서 "watermelon"이 아닌 "go" 같이 더 짧은 경우도 대비  
            memcpy(&dst_buf[next_read_fpos + (idx * diff_len) + dst_len], tmp_buf, src_len - abs(diff_len));
            memset(tmp_buf, 0, src_len - abs(diff_len));

            // *2. "apple" 이라는 단어를 찾아서 처리 후에는 파일 커서를 +1 해서 또 찾으면 불필요한 반복이 진행될 수 있음 
            //     apple 만큼 증가한 곳에 다음 파일 커서를 위치시키고 위의 과정을 반복한다.
            next_read_fpos = curr_read_fpos;

            printf("-------------------------------------------------------------\n");
            printf("dst_buf : %s\n", dst_buf);
            printf("-------------------------------------------------------------\n");

            idx++;

        }
        // #4. 다르다면 파일 커서를 하나씩 증가시키며 "apple" 찾기 반복
        else {
            next_read_fpos = lseek(fd_src, 0, SEEK_CUR) - cmp_len + 1;
        }

        printf("next_read_fpos : %ld\n", next_read_fpos);
    }

    close(fd_src);

/* 단어가 변경된 문자열을 파일에 저장 */
    if( (fd_dst = open("dst_file.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644)) == ERROR ) {
        perror("open error!\n");
        exit(1);
    }

    dst_len = strlen(dst_buf);

    if ( (write_bytes = write(fd_dst, dst_buf, dst_len)) == ERROR ) {
        perror("write error!\n");
        close(fd_dst);
        exit(1);
    } else if( write_bytes != dst_len ) {
        printf("didn't write all\n");
        close(fd_dst);
        exit(1);
    }

    close (fd_dst);

    return 0;
}