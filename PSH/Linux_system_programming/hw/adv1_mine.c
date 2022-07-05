#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define ERROR               -1
#define SLAB_SIZE		    64
#define BUDDY_PAGE_SIZE		4096

char src_buf[SLAB_SIZE] = "apple test apple test apple test";
char cmp_buf[SLAB_SIZE] = { 0 };
char dst_buf[SLAB_SIZE] = { 0 };

int src_len;
int cmp_len;
int dst_len;

int fd_src;
int fd_dst;

off_t next_read_fpos = 0; 
/*
 * 사용법
 * argv[1] = 저장 파일
 * argv[2] = 바뀔 단어   
 * argv[3] = 바꿀 단어 
 */

/*
### To Do ###
1. 
*/

int main(int argc, char **argv)
{
    int write_bytes, read_bytes, read_remain_bytes;
    int i, idx;

    src_len = strlen(src_buf);

    if( argc < 4) {
       printf("argument input error!\n"); 
       return -1;
    }

    if( (fd_src = open(argv[1], O_RDWR | O_TRUNC | O_CREAT, 0644)) == ERROR ) {
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

    if( (read_bytes = read(fd_src, src_buf, write_bytes)) == ERROR ) {
        perror("read error!\n");
        close(fd_src);
        exit(1);
    } else if( read_bytes != write_bytes ) {
        printf("didn't read all\n");
        close(fd_src);
        exit(1);
    }

    printf("src_buf : %s\n", src_buf);

    lseek(fd_src, (off_t) 0, SEEK_SET);

    cmp_len = strlen(argv[2]);
    dst_len = strlen(argv[3]);

    for(i = 0; i < 10; i++)
    {
        lseek(fd_src, (off_t)next_read_fpos, SEEK_SET);

        // if( (read_bytes = read(fd_src, cmp_buf, cmp_len)) == ERROR ) {
        //     perror("read error!\n");
        //     close(fd_src);
        //     exit(1);
        // }

        if( !(strncmp(&src_buf[i], argv[2], src_len)) ) {

            next_read_fpos = lseek(fd_src, 0, SEEK_CUR);

            strncpy(dst_buf, argv[3], dst_len);
            printf("dst_len = %d\n", dst_len);

            read_remain_bytes = read(fd_src, &dst_buf[dst_len], SLAB_SIZE);
            printf("dst_buf : %s\n", dst_buf);
        }
        else {
            next_read_fpos = lseek(fd_src, 0, SEEK_CUR) - cmp_len + 1;
        }

        printf("next_read_fpos : %ld\n", next_read_fpos);
    }

#if 0

    lseek(fd_src, 0, SEEK_SET);

    for( i = 0 ; i < src_len; i++ ) {
        lseek(fd_src, next_read_fpos, SEEK_SET);

        if( (read_bytes = read(fd_src, cmp_buf, cmp_len)) == ERROR ) {
            perror("read error!\n");
            close(fd_src);
            exit(1);
        } 

        if( !(strncmp(cmp_buf, argv[3], cmp_len)) ) {
            strncpy(dst_buf, argv[4], strlen(argv[4]));
            // printf("dst_buf : %s\n", dst_buf);

            next_read_fpos = lseek(fd_src, 0, SEEK_CUR);

            read_remain_bytes = read(fd_src, tmp_buf, SLAB_SIZE);
            strncat(dst_buf, tmp_buf, read_remain_bytes);
            printf("merged_dst_buf : %s\n", dst_buf);

            i = i + cmp_len - 1;
        }
        else {
            next_read_fpos += 1;
        }

    }
#endif
    return 0;
}