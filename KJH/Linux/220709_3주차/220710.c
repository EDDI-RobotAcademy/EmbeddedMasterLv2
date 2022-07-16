/*

과제 : 
디렉토리 순회가 되니까 지금까지 배운것들을 융복합시킨다 하면 어떤게 제일아름다울까?
자료구조까지 딱 써야하니까 슬슬 트리도 넣어봅시다
RB 트리에 쓰레드까지넣으면 실무에서 쓸수잇는 구조인데
이거랑 쓰레드를 합치면 재미있는걸만들수있다.
시그널도 합치면 재미있다.

시그널하는게
1초마다 랜덤 난수를 만들어서 큐를 올린다.
그래서 rb트리를 써서 반복적으로 들어오는애들은 freq를 높히고 낮추고
캐싱을 해야되는정보 안해야되는정보들을
챱챱한다.
접근이 많은 자료들을 분류할때 쓴다.

스마트시티 디렉토리내 커버리지 분석
코드레벨 라인수
c코드 개수
java 코드 개수 등등
디렉토리 순회 및 wc 해야한다.
전체 c코드 라인수
전체 자바코드 라인수
h

재귀호출을 없앤다.
스왑이라는건 다음주에 한다.

*/

/*
0. 재귀 형식이 아닌 다른 방법을 이용해 탐색해야 세그먼테이션 폴트가 나지않는다. (스택 폭파)
0. 트리구조를 이용해 구현하자
0. 트리구조를 이요해 구현하지 않아도 된다.
*/

/*
1. 파일 용량을 확인한다.
2. 파일 용량을 확인한뒤 PageSize(4096Byte)로 나눠 파일을 각각 읽어온다.
3. 읽어 온 파일에서 개행 문자('\n')가 몇개 포함되어있는지 모두 더한다.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// ls_adv_stat.c
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define TAB_SIZE	8

typedef struct DIR_STRUCT{
	int data;
	struct DIR_STRUCT *link;
}dir_struct;

void recursive_dir(char *dname);
void example (char *dir_name);

int main(int argc, char **argv)
{
	dir_struct dir1;
	//recursive_dir(argv[1]);
	recursive_dir(".");

	return 0;
}

void recursive_dir(char *dname)
{
	DIR *dp;
	struct stat buf;
	struct dirent *p;

	int max_tab = 0;

	chdir(dname);
	dp = opendir(".");
	printf("==================\n%s: \n", dname);


	// calc_max_tab
	while (p = readdir(dp))
	{
		if(max_tab < strlen(p->d_name))
		{
			max_tab = strlen(p->d_name);
		}	
	}
	if(max_tab%TAB_SIZE){
		max_tab = max_tab/TAB_SIZE;
		max_tab++;
	}
	else
	{
		max_tab = max_tab/TAB_SIZE;
	}
	
	rewinddir(dp);

	// calc_max_tab


	while (p = readdir(dp))
	{
		printf("%s", p->d_name);

		for(int tab = strlen(p->d_name)/TAB_SIZE ; tab < max_tab ; tab++)
		{
			printf("\t");
		}
		example(p->d_name);
	}

	rewinddir(dp);

	while (p = readdir(dp))
	{
		stat(p->d_name, &buf);

		if (S_ISDIR(buf.st_mode))
		{
			if (strcmp(p->d_name, ".") && strcmp(p->d_name, ".."))
			{
				recursive_dir(p->d_name);
			}
		}
	}

	chdir("..");

	closedir(dp);
}

void example (char *dir_name)
{
	// ls_adv_stat.c
	struct stat buf;
	struct passwd *pw;
	struct group *gr;
	struct tm *tm;

	char perm[11] = "----------";
	char rwx[4] = "rwx";
	char ch;

	int i;

	// if (argc != 2)
	// {
	// 	printf("사용법: 분석할파일명");
	// 	return 0;
	// }

	stat(dir_name, &buf);

	if (S_ISDIR(buf.st_mode))
		ch = 'd';
	else if (S_ISREG(buf.st_mode))
		ch = '-';
	else if (S_ISFIFO(buf.st_mode))
		ch = 'p';
	else if (S_ISLNK(buf.st_mode))
		ch = 'l';
	else if (S_ISSOCK(buf.st_mode))
		ch = 's';
	else if (S_ISCHR(buf.st_mode))
		ch = 'c';
	else if (S_ISBLK(buf.st_mode))
		ch = 'b';

	for (i = 0; i < 9; i++)
	{
		if ((buf.st_mode >> (8 - i)) & 1)
		{
			perm[i + 1] = rwx[i % 3];
		}
	}

	printf("%s ", perm);
	printf("%ld ", buf.st_nlink);
	pw = getpwuid(buf.st_uid);
	printf("%s ", pw->pw_name);
	gr = getgrgid(buf.st_gid);
	printf("%s ", gr->gr_name);
	printf("%ld ", buf.st_size);

	tm = localtime(&buf.st_mtime);
	printf("%d-%02d-%02d %02d:%02d:%02d",
			tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);

	printf("\n");
}