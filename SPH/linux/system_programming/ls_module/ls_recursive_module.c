#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUDDY_PAGE_SIZE 4096
#define ONEBYTE 1

typedef struct _line_cnt line_cnt;
struct _line_cnt{
	int c;
};

typedef enum _file_ext file_ext;
enum{
	c = 0,
	js,
	v,
	eof
};

line_cnt line_value = {0,};

extern inline int get_name_size(char *str)
{
	int i = 0;
	int tot_size = ONEBYTE;

	while(str[i])
	{
		i++;
		tot_size++;
	}

	return tot_size - ONEBYTE;
}

bool check_file_ext(char *p_name, char *ext)
{
	static int p_name_size;
	static int ext_size;
	int offset;

	p_name_size = get_name_size(p_name);
	ext_size = get_name_size(ext);

	offset = p_name_size - ext_size;

	if(!strcmp(p_name+offset, ext))
		return true;
	else
		return false;
}

void cnt_file_line(char *f_name)
{
	int fd;
	int i = 0;
	char ch;

	fd = open(f_name, O_RDONLY);

	if(!fd)
	{
		printf("파일열기 실패\n");
		return;
	}

	while(read(fd, &ch, 1)) 
	{
		if(ch == '\n')
			line_value.c++;
	}
}

#if 1
void check_file_type(DIR *dp, char *f_ext)
{
	struct dirent *p;
	struct stat st;

	while(p = readdir(dp))
	{
		//숨긴 파일은 제외한다
		if(p->d_name[0] == '.')
			continue;
		//현재 디렉토리 파일의 정보를 얻는다
		stat(p->d_name, &st);

		if(S_ISREG(st.st_mode) && check_file_ext(p->d_name, f_ext))
		{
			printf("file : %s\n", p->d_name);
			cnt_file_line(p->d_name);
			printf("line = %d\n", line_value.c);
		}
	}
}
#endif

void recursive_dir_traversal(char *d_name, char *f_ext)
{
	DIR *dp;
	struct dirent *p;
	struct stat st_buf;

	//작업 디렉토리를 입력받은 디렉토리로 변경한다
	chdir(d_name);
	//현재 작업 디렉토리를 열고 디렉토리 포인터를 얻는다
	dp = opendir(".");

	//예외처리 : 디렉토리가 없는 경우
	if(!dp)
	{
		printf("디렉토리가 없습니다\n");
		return;
	}

	//printf("dir : %s\n", d_name);

	//check_file_type(dp, f_ext);

	while(p = readdir(dp))
	{
		if(p->d_name[0] == '.')
			continue;
		stat(p->d_name, &st_buf);

		if(S_ISDIR(st_buf.st_mode))
		{
			if(strcmp(p->d_name, ".") && strcmp(p->d_name, ".."))
			{
				recursive_dir_traversal(p->d_name, f_ext);
			}
		}
	}


	rewinddir(dp);
#if 0
	while(p = readdir(dp))
	{
		if(p->d_name[0] == '.')
			continue;
		stat(p->d_name, &st_buf);

		if(S_ISDIR(st_buf.st_mode))
		{
			if(strcmp(p->d_name, ".") && strcmp(p->d_name, ".."))
			{
				recursive_dir_traversal(p->d_name, f_ext);
			}
		}
	}
#endif
	check_file_type(dp, f_ext);

	chdir("..");

	closedir(dp);
}

/*
   목적 : Smart-City 프로젝트 디렉토리내의 모든 c파일과 js파일, v파일의 라인수를 확인한다
*/
int main(int argc, char **argv)
{
#if 1
	if(argc < 3)
	{
		printf("사용법 : ./실행파일 디렉토리경로 확장자(예 : .c)\n");
		exit(1);
	}

	recursive_dir_traversal(argv[1], argv[2]);
#endif
	//recursive_dir_traversal(argv[1], ".c");

	printf("%s line : %d\n", argv[2], (int)line_value.c);
	return 0;
}
