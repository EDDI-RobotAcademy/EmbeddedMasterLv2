/*TODO : 표준 라이브러리와 공통으로 사용되는 define의 경우 common으로 분리한다*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ONEBYTE 1

#define STRMATCH 0
#define STRUNMATCH 1
/******************************************************************************/

typedef enum _code_type code_type;
enum _code_type{
	c = 0,
	fpga,
	_asm,
	java,
	vue
};

typedef struct _c_line_var c_line_var;
struct _c_line_var{
	int c;
	int h;
};

typedef struct _fpga_line_var fpga_line_var;
struct _fpga_line_var{
	int v;
	int vhdl;
};

typedef struct _line_var line_var;
struct _line_var{
	c_line_var c;
	fpga_line_var fpga;
	int _asm;
	int java;
	int vue;
};

extern inline int get_name_size(char *str)
{
	int i = 0;
	//int tot_size = (int)ONEBYTE;
	int tot_size = 0;

	while(str[i])
	{
		i++;
		tot_size++;
	}

	return tot_size - (int)ONEBYTE;
}

extern inline bool check_file_ext(char *d_name, char *ext)
{
	int d_name_size = 0;
	int ext_size = 0;
	int offset = 0;

	d_name_size = get_name_size(d_name);
	ext_size = get_name_size(ext);

	offset = d_name_size - ext_size;

	if(!strcmp(d_name+offset, ext))
		return true;
	else
		return false;
}

int chk_ext_cnt_line(char *d_name, char *ext)
{
	int fd, i, line;
	char ch;

	i = 0;
	line = 0;

	if(!check_file_ext(d_name, ext))
		return 0;

	fd = open(d_name, O_RDONLY);

	if(!fd)
	{
		printf("파일열기 실패\n");
		exit(3);
	}

	while(read(fd, &ch, ONEBYTE)) 
	{
		if(ch == '\n')
			line++;
	}

	//issue 1 : 프로세스 멈춤 해결
	//파일을 연상태에서 닫지 않고 계속 파일을 열어 버퍼 터짐(?)
	close(fd);

	return line;
}

void recur_chk_code_coverage(char *origin_dir, char *tmp_dir, char *f_ext, int *line_buf)
{
	DIR *dp;
	struct dirent *p;
	struct stat st_buf;

	//작업 디렉토리를 입력받은 디렉토리로 변경한다
	chdir(tmp_dir);
	//현재 작업 디렉토리를 열고 디렉토리 포인터를 얻는다
	dp = opendir(".");

	//예외처리 : 디렉토리가 없는 경우
	if(!dp)
	{
		printf("디렉토리가 없습니다\n");
		exit(2);
	}

	while(p = readdir(dp))
	{
		if(p->d_name[0] == '.')
			continue;

		stat(p->d_name, &st_buf);

		if(S_ISREG(st_buf.st_mode))
		{
			*line_buf += chk_ext_cnt_line(p->d_name, f_ext);
		}
	}

	rewinddir(dp);

	//파일의 형식이 디렉토리 파일인지 확인한다
	//디렉토리 파일이 아니라면 루프 탈출, 디렉토리 파일이면 재귀 호출로 작업 디렉토리를 옮긴다
	while(p = readdir(dp))
	{
		if(p->d_name[0] == '.')
			continue;
		stat(p->d_name, &st_buf);

		if(S_ISDIR(st_buf.st_mode))
		{
			//숨긴 파일이 아니라면 재귀 호출
			if(strcmp(p->d_name, ".") && strcmp(p->d_name, ".."))
				recur_chk_code_coverage(origin_dir, p->d_name, f_ext, line_buf);
		}
	}

	//issue 2 : dp가 origin dir에 갔을 때 예외처리 
	if(!strcmp(origin_dir, tmp_dir))
	{
		closedir(dp);
		return;
	}
	chdir("..");
	closedir(dp);
}

void run_code_coverage(char *path, code_type type)
{
	line_var *line = malloc(sizeof(line_var));
	memset(line, 0, sizeof(line_var));
	char *tmp = path;

	switch(type)
	{
		case c:
			recur_chk_code_coverage(path, tmp, ".c", &line->c.c);
			recur_chk_code_coverage(path, tmp, ".h", &line->c.h);
			printf("C코드 라인 : %d\n", line->c.c + line->c.h);
			break;
		case fpga:
			recur_chk_code_coverage(path, tmp, ".v", &line->fpga.v);
			//issue 3 : 오타 수정
			recur_chk_code_coverage(path, tmp, ".vhdl", &line->fpga.vhdl);
			printf("FPGA(VHDL)코드 라인 : %d\n", line->fpga.v+line->fpga.vhdl);
			break;
		case _asm:
			recur_chk_code_coverage(path, tmp, ".asm", &line->_asm);
			printf("어셈블리코드 라인 : %d\n", line->_asm);
			break;
		case java:
			recur_chk_code_coverage(path, tmp, ".java", &line->java);
			printf("java코드 라인 : %d\n", line->java);
			break;
		case vue:
			recur_chk_code_coverage(path, tmp, ".vue", &line->vue);
			printf("vue코드 라인 : %d\n", line->vue);
			break;
	}
	free(line);
}

/*
   목적 : Smart-City 프로젝트 디렉토리내의 모든 c코드, java코드, fpga코드,
          assembly코드, vue코드 파일의 라인수를 확인한다
   TODO
   1. 탐색하고자 하는 디렉토리를 입력받는다
   *예외 처리 : 명령인수가 2보다 작으면 프로세스 종료
   2. 재귀방식으로 
*/
int main(int argc, char **argv)
{
	int i;
	int line;

	if(argc < 2)
	{
		printf("사용법 : ./실행파일 디렉토리경로\n");
		exit(1);
	}

	run_code_coverage(argv[1], c);
	run_code_coverage(argv[1], fpga);
	run_code_coverage(argv[1], _asm);
	run_code_coverage(argv[1], java);
	run_code_coverage(argv[1], vue);

	return 0;
}
