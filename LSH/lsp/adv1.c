#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>

#define SLABSIZE		64

int src_start_loc[SLABSIZE] = { 0 };
int src_loc_cnt = 0;

int dst_start_loc[SLABSIZE] = { 0 };
int dst_loc_cnt = 0;

char *apple = "apple";
char *watermelon = "watermelon";

int srclen;
int dstlen;

int main(void)
{
	int i;
	int cnt = 0;
	int read_byte;
	int addsize = 0;

	char buf[SLABSIZE] = { 0 };
	char tmp_buf[SLABSIZE] = { 0 };
	char preset[SLABSIZE] = "apple test apple test apple test\n";

	srclen = strlen(apple);
	dstlen = strlen(watermelon);

	int rwfd = open("test.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);

	write(rwfd, preset, strlen(preset));
	lseek(rwfd, 0, SEEK_SET);

	for (i = 0; i < strlen(preset) + addsize; i++)
	{
		lseek(rwfd, i, SEEK_SET);

		if (read(rwfd, buf, srclen) == srclen && !strncmp(buf, apple, srclen))
		{
			cnt++;
			read_byte = read(rwfd, tmp_buf, SLABSIZE);
			lseek(rwfd, i, SEEK_SET);
			write(rwfd, watermelon, dstlen);
			write(rwfd, tmp_buf, read_byte);
			addsize = (dstlen - srclen) * cnt;
		}
	}

	return 0;
}
