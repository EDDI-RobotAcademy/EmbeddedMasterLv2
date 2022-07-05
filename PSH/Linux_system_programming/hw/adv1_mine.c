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
int diff_len;

int fd_src;
int fd_dst;

off_t next_read_fpos = 0; 
off_t curr_read_fpos = 0; 

/*
 * argv[1] = 바뀔 단어
 * argv[2] = 바꿀 단어   
 */

int main(int argc, char **argv)
{
    int write_bytes, read_bytes, read_remain_bytes;
    volatile int i, idx=0;

    src_len = strlen(src_buf);

    if( argc < 3) {
       printf("argument input error!\n"); 
       return -1;
    }

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

    for(i = 0; i < (src_len - cmp_len - (idx*cmp_len) + 1); i++)
    {
        // printf("cnt : %d\n", (src_len - cmp_len - (idx*cmp_len) + 1));
        lseek(fd_src, (off_t)next_read_fpos, SEEK_SET);

        if( (read_bytes = read(fd_src, cmp_buf, cmp_len)) == ERROR ) {
            perror("read error!\n");
            close(fd_src);
            exit(1);
        }

        if( !(strncmp(cmp_buf, argv[1], cmp_len)) ) {
            // printf("dst_read_fpos_1 : %ld\n", next_read_fpos + ((idx) * diff_len));
            strncpy(&dst_buf[next_read_fpos + (idx * diff_len)], argv[2], dst_len);

            curr_read_fpos = lseek(fd_src, 0, SEEK_CUR);

            // printf("dst_read_fpos_2 : %ld\n", next_read_fpos + (idx * diff_len) + dst_len);
            read_remain_bytes = read(fd_src, &dst_buf[next_read_fpos + (idx * diff_len) + dst_len], SLAB_SIZE);

            next_read_fpos = curr_read_fpos;

            printf("-------------------------------------------------------------\n");
            printf("dst_buf : %s\n", dst_buf);
            printf("-------------------------------------------------------------\n");

            idx++;
        }
        else {
            next_read_fpos = lseek(fd_src, 0, SEEK_CUR) - cmp_len + 1;
        }

        printf("next_read_fpos : %ld\n", next_read_fpos);
    }

    close(fd_src);

    if( (fd_dst = open("dst_file.txt", O_RDWR | O_TRUNC | O_CREAT, 0644)) == ERROR ) {
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