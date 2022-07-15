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

typedef struct _dp_stack dp_stack;
struct _dp_stack{
	DIR *dp;
	dp_stack *link;
};

dp_stack *create_dp_stack_node(dp_stack **top, DIR *dp)
{
	dp_stack *tmp = NULL;

	tmp = (dp_stack *)malloc(sizeof(dp_stack));
	tmp->dp = dp;
	tmp->link = *top;

	return tmp;
}

void push_dp_stack(dp_stack **top, DIR *dp)
{
	dp_stack *tmp = *top;
	*top = create_dp_stack_node(&tmp, dp);
}

DIR *pop_dp_stack(dp_stack **top)
{
	dp_stack *tmp;
	DIR *dp;

	tmp = *top;

	if(!tmp)
	{
		return NULL;
	}

	dp = tmp->dp;
	*top = tmp->link;

	free(tmp);

	return dp;
}

void mv_dp_to_dirtype(DIR *dp)
{
	struct dirent *p;
	struct stat st;

	while(p = readdir(dp))
	{
		if(p->d_name[0] == '.')
			continue;
		stat(p->d_name, &st);

		if(S_ISDIR(st.st_mode))
			break;
	}
}

void read_dir_contents(DIR *dp)
{
	struct dirent *p;
	struct stat st;

	while(p = readdir(dp))
	{
		if(p->d_name[0] == '.')
			continue;
		printf("contents : %s\n", p->d_name);
	}
}
/*
   목적 : Smart-City 프로젝트 디렉토리내의 모든 c파일과 js파일, v파일의 라인수를 확인한다
   1. 
*/
int main(int argc, char **argv)
{
	DIR *dp = NULL;
	DIR *loop;
	int fd;
	char buf[BUDDY_PAGE_SIZE];
	struct dirent *p;
	struct stat st_buf;
	dp_stack *top = NULL;
	bool dir_flag = true;

	if(chdir("../../../../../../Smart-City"))
	{
		printf("change working directory fail\n");
		exit(1);
	}

	dp = opendir(".");
	if(!dp)
	{
		printf("there is no directory\n");
		exit(1);
	}

	//dp가 NULL을 가리킬 때까지 반복한다
	while (p = readdir(dp))
	{
		if(p->d_name[0] == '.')
			continue;
#if 0	
		stat(p->d_name, &st_buf);
		if(S_ISDIR(st_buf.st_mode))
		{
			if(strcmp(p->d_name, ".") && strcmp(p->d_name, ".."))
			{
				printf(".dir : %s\n", p->d_name);
				//push_dp_stack(&top, dp);
				chdir(p->d_name);
				dp = opendir(".");
			}
		}
#endif
#if 0
		else
		{
			rewinddir(dp);
			printf("file : %s\n", p->d_name);
			chdir("..");
			dp = opendir(".");
			dp = pop_dp_stack(&top);
		}

		if(!dp)
			break;
#endif
	}

	closedir(dp);

	return 0;
}
