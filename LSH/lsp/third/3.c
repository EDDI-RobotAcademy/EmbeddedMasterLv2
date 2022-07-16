#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

// argv[1] = ~/proj/eddi/academy/Smart-City/
#define BUDDY_SIZE		4096

/*
1. 전체 디렉토리를 순회하며 디렉토리를 다중 트리화 한다.
2. 다음엔 검색하지 않도록 메타화 한다.
3. 저장한 정보를 바탕으로 각 코드를 순회하며 라인수, 파일수를 체크한다.
*/

#define TYPE_C						2
#define TYPE_H						2
#define TYPE_CPP					4
#define TYPE_HPP					4
#define TYPE_JAVA					5
#define TYPE_VUE					4
#define TYPE_S						2
#define TYPE_ASM					4
#define TYPE_SH						3
#define TYPE_VHDL					5
#define TYPE_VERILOG				8

#define ARRAY_START_ADJUST_IDX		1
#define READ_ONE_BYTE				1

typedef struct _code_coverage
{
	int line;
} code_coverage;

code_coverage c_code;
code_coverage java_code;
code_coverage vue_code;
code_coverage js_code;
code_coverage asm_code;
code_coverage sh_code;
code_coverage vhdl_code;

void analysis_file(int fd, char *ch, int byte, code_coverage *cc)
{
	bool flag = false;

    while (read(fd, ch, byte))
    {
        if (*ch == '\n')
        {
            cc->line++;
        }
        else if (*ch != '\t' && *ch != ' ')
        {
            if (flag == false)
            {
                flag = true;
            }
        }
        else
        {
            flag = false;
        }
    }
}

void analysis_code(char *path)
{
	int fd;
	char ch;
	int pathlen = strlen(path);

	if ((!strncmp(&path[pathlen - TYPE_C], ".c", TYPE_C)) ||
		(!strncmp(&path[pathlen - TYPE_H], ".h", TYPE_H)) ||
		(!strncmp(&path[pathlen - TYPE_CPP], ".cpp", TYPE_CPP)) ||
		(!strncmp(&path[pathlen - TYPE_H], ".hpp", TYPE_HPP)))
	{
		if ((fd = open(path, O_RDONLY)) == -1)
		{
			exit(1);
		}

		analysis_file(fd, &ch, READ_ONE_BYTE, &c_code);

		close(fd);
	}
	else if (!strncmp(&path[pathlen - TYPE_JAVA], ".java", TYPE_JAVA))
	{
		if ((fd = open(path, O_RDONLY)) == -1)
		{
			exit(1);
		}

		analysis_file(fd, &ch, READ_ONE_BYTE, &java_code);

		close(fd);
	}
#if 0
	else if (!strncmp(&path[pathlen - TYPE_JS], ".js", TYPE_JS))
	{
		if ((fd = open(path, O_RDONLY)) == -1)
		{
			exit(1);
		}

		analysis_file(fd, &ch, READ_ONE_BYTE, &js_code);

		close(fd);
	}
#endif
	else if (!strncmp(&path[pathlen - TYPE_VUE], ".vue", TYPE_VUE))
	{
		if ((fd = open(path, O_RDONLY)) == -1)
		{
			exit(1);
		}

		analysis_file(fd, &ch, READ_ONE_BYTE, &vue_code);

		close(fd);
	}
	else if (!strncmp(&path[pathlen - TYPE_ASM], ".asm", TYPE_ASM) || 
			(!strncmp(&path[pathlen - TYPE_ASM], ".s", TYPE_ASM)) || 
			(!strncmp(&path[pathlen - TYPE_ASM], ".S", TYPE_S)))
	{
		if ((fd = open(path, O_RDONLY)) == -1)
		{
			exit(1);
		}

		analysis_file(fd, &ch, READ_ONE_BYTE, &asm_code);

		close(fd);
	}
	else if (!strncmp(&path[pathlen - TYPE_SH], ".sh", TYPE_SH))
	{
		if ((fd = open(path, O_RDONLY)) == -1)
		{
			exit(1);
		}

		analysis_file(fd, &ch, READ_ONE_BYTE, &sh_code);

		close(fd);
	}
	else if (!strncmp(&path[pathlen - TYPE_VHDL], ".vhdl", TYPE_VHDL))
	{
		if ((fd = open(path, O_RDONLY)) == -1)
		{
			exit(1);
		}

		analysis_file(fd, &ch, READ_ONE_BYTE, &vhdl_code);

		close(fd);
	}

}

void analysis_code_coverage(char *path)
{
	DIR *dp;
	struct stat buf;
	struct dirent *p;

	chdir(path);
	dp = opendir(".");

	while (p = readdir(dp))
	{
		stat(p->d_name, &buf);

		if (S_ISDIR(buf.st_mode))
		{
			if (strcmp(p->d_name, ".") && strcmp(p->d_name, ".."))
			{
				analysis_code_coverage(p->d_name);
			}
		}
		else if (S_ISREG(buf.st_mode))
		{
			analysis_code(p->d_name);
		}
	}

	chdir("..");
	closedir(dp);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("사용법: 분석할 디렉토리명을 인자로 입력하세요!\n");
		exit(-1);
	}

	analysis_code_coverage(argv[1]);
	printf("c 코드 라인: %d\n", c_code.line);
	printf("java 코드 라인: %d\n", java_code.line);
	//printf("js 코드 라인: %d\n", js_code.line);
	printf("vue 코드 라인: %d\n", vue_code.line);
	printf("어셈블리 코드 라인: %d\n", asm_code.line);
	printf("도커 및 환경 설정 자동화 코드 라인: %d\n", sh_code.line);
	printf("FPGA(VHDL) 코드 라인: %d\n", vhdl_code.line);

	return 0;
}
