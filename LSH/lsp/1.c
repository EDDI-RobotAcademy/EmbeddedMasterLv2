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

void find_src_location(char *buf, char *src, int size)
{
	int i;
	int srclen = strlen(src);

	for (i = 0; i < size; i++)
	{
		if (!strncmp(&buf[i], src, srclen))
		{
			src_start_loc[src_loc_cnt++] = i;
		}
	}
}

void print_src_loc (void)
{
	int i;

	for (i = 0; i < src_loc_cnt; i++)
	{
		printf("%3d\n", src_start_loc[i]);
	}
}

void check_dst_location(char *buf, char *src, char *dst, int size)
{
	int i;
	int srclen = strlen(src);
	int dstlen = strlen(dst);
	int difflen = dstlen - srclen;

	for (i = 0; i < src_loc_cnt; i++)
	{
		dst_start_loc[dst_loc_cnt++] = src_start_loc[i] + difflen * i;
	}
}

void print_dst_loc (void)
{
	int i;

	for (i = 0; i < dst_loc_cnt; i++)
	{
		printf("%3d\n", dst_start_loc[i]);
	}
}

void change_word(char *buf, char *src, char *dst, int size)
{
	int i;
	int srclen = strlen(src);
	int dstlen = strlen(dst);

	int locdiff;

	//0          11         22
	//apple test apple test apple test\n
	//0               16              32
	//watermelon test watermelon test watermelon test\n
	//apple      test apple test apple test\n

	//apple testtesttest apple testtest apple test\n
	//watermelon testtesttest watermelon testtest watermelon test\n
	//apple      testtesttest apple testtest apple test\n

	for (i = 0; i < src_loc_cnt; i++)
	{
		locdiff = size - (src_start_loc[i] + srclen);

		memmove(&buf[dst_start_loc[i] + dstlen],
				&buf[dst_start_loc[i] + srclen],
				locdiff);
		memmove(&buf[dst_start_loc[i]], dst, dstlen);
	}
}

int main(void)
{
	int read_byte;

	char buf[SLABSIZE] = { 0 };
	char preset[SLABSIZE] = "apple test apple test apple test\n";

	int rwfd = open("test.txt", O_RDWR | O_TRUNC | O_CREAT, 0644);

	write(rwfd, preset, strlen(preset));
	lseek(rwfd, 0, SEEK_SET);
	read_byte = read(rwfd, buf, SLABSIZE);

	find_src_location(buf, "apple", read_byte);
	print_src_loc();

	check_dst_location(buf, "apple", "watermelon", read_byte);
	print_dst_loc();

	change_word(buf, "apple", "watermelon", read_byte);
	write(rwfd, buf, strlen(buf));

	printf("%s", buf);

	return 0;
}
