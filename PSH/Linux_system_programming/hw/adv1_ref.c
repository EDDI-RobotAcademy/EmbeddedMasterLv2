#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

#define SLABSIZE		64

int src_start_loc[SLABSIZE] = { 0 };
int src_loc_cnt = 0;

int dst_start_loc[SLABSIZE] = { 0 };
int dst_loc_cnt = 0;

char *apple = "apple";
char *watermelon = "watermelon";
char *go = "go";

int srclen;
int dstlen;
int diff_len;

int main(void)
{
	int i;
	int cnt = 0;
	int read_byte;
	int addsize = 0;
	off_t tmp_pos;

	char buf[SLABSIZE] = { 0 };
	char tmp_buf[SLABSIZE] = { 0 };
	char preset[SLABSIZE] = "apple test apple test apple test";

	srclen = strlen(apple);
	// dstlen = strlen(watermelon);
	dstlen = strlen(go);
	diff_len = abs(srclen-dstlen);

	int rwfd = open("test.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);

	write(rwfd, preset, strlen(preset));
	lseek(rwfd, 0, SEEK_SET);

	for (i = 0; i < strlen(preset) + addsize; i++)
	{
		lseek(rwfd, i, SEEK_SET);

		if (read(rwfd, buf, srclen) == srclen && !strncmp(buf, apple, srclen))
		{
			memset(tmp_buf, 0x00, SLABSIZE);
			read_byte = read(rwfd, tmp_buf, SLABSIZE);
			printf("read_byte =%d\n", read_byte);
			printf("tmp_buf =%s\n", tmp_buf);

			lseek(rwfd, i, SEEK_SET);

			// write(rwfd, watermelon, dstlen);
			write(rwfd, go, dstlen);

			// tmp_pos = lseek(rwfd, 0, SEEK_CUR);

			memset(&tmp_buf[read_byte], NULL, diff_len * cnt);
			write(rwfd, tmp_buf, read_byte + (diff_len * cnt));

			// lseek(rwfd, tmp_pos, SEEK_SET);
			// memset(tmp_buf, 0x00, read_byte);
			// write(rwfd, tmp_buf, read_byte);

			cnt++;
			addsize = (dstlen - srclen) * cnt;
		}
	}

	return 0;
}
