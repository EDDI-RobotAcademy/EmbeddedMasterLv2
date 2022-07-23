#include <stdio.h>
#include <time.h>

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MANAGE_NUMBER			5
#define SLAB_SIZE				64

#define PROCESS_ALL_OK			33

#define CAMERA					1
#define LIDAR					2
#define CONTROL					3
#define RADAR					4
#define LTE						5

typedef struct _managed_process managed_process;

struct _managed_process
{
	int cpid;
	char name[SLAB_SIZE];
};

typedef struct _process_control_block process_control_block;

struct _process_control_block
{
	struct _managed_process *managed_process;
	int current_alloc_num;
};

process_control_block *init_process_control_block(void)
{
	process_control_block *pcb = (process_control_block *)malloc(sizeof(process_control_block));

	pcb->managed_process = (managed_process *)malloc(sizeof(managed_process) * MANAGE_NUMBER);
	pcb->current_alloc_num = 0;
}

int analysis_user_input(char *buf)
{
    read(0, buf, SLAB_SIZE);
    return atoi(buf);
}

void create_camera_process(process_control_block *pcb)
{
	int tmp_id = pcb->managed_process[pcb->current_alloc_num].cpid = fork();

	if (tmp_id)
	{
		printf("I'm parent(process control manager) - (pid: %d), (cpid: %d)\n", getpid(), tmp_id);
		pcb->current_alloc_num++;
	}
	else if (!tmp_id)
	{
		strncpy(pcb->managed_process[pcb->current_alloc_num].name, "camera", 7);
		printf("I'm child(%s process) - (pid: %d), (cpid: %d)\n",
				pcb->managed_process->name, getpid(), tmp_id);

		for (;;)
		{
			printf("카메라 처리중\n");
			sleep(1);
		}
	}
}

void create_lidar_process(process_control_block *pcb)
{
	int tmp_id = pcb->managed_process[pcb->current_alloc_num].cpid = fork();

	if (tmp_id)
	{
		printf("I'm parent(process control manager) - (pid: %d), (cpid: %d)\n", getpid(), tmp_id);
		pcb->current_alloc_num++;
	}
	else if (!tmp_id)
	{
		strncpy(pcb->managed_process->name, "lidar", 6);
		printf("I'm child(%s process) - (pid: %d), (cpid: %d)\n",
				pcb->managed_process->name, getpid(), tmp_id);

		for (;;)
		{
			printf("라이다 처리중\n");
			sleep(1);
		}
	}
}

void create_control_process(process_control_block *pcb)
{
	int tmp_id = pcb->managed_process[pcb->current_alloc_num].cpid = fork();

	if (tmp_id)
	{
		printf("I'm parent(process control manager) - (pid: %d), (cpid: %d)\n", getpid(), tmp_id);
		pcb->current_alloc_num++;
	}
	else if (!tmp_id)
	{
		strncpy(pcb->managed_process->name, "control", 8);
		printf("I'm child(%s process) - (pid: %d), (cpid: %d)\n",
				pcb->managed_process->name, getpid(), tmp_id);

		for (;;)
		{
			printf("차량 제어중\n");
			sleep(1);
		}
	}
}

void create_radar_process(process_control_block *pcb)
{
	int tmp_id = pcb->managed_process[pcb->current_alloc_num].cpid = fork();

	if (tmp_id)
	{
		printf("I'm parent(process control manager) - (pid: %d), (cpid: %d)\n", getpid(), tmp_id);
		pcb->current_alloc_num++;
	}
	else if (!tmp_id)
	{
		int i;
		strncpy(pcb->managed_process->name, "radar", 6);
		printf("I'm child(%s process) - (pid: %d), (cpid: %d)\n",
				pcb->managed_process->name, getpid(), tmp_id);

		sleep(1);

		for (i = 0; i < 1000000; i++)
		{
			printf("간드아: %d\n", i + 1);
		}

		exit(PROCESS_ALL_OK);
	}
}

void create_lte_process(process_control_block *pcb)
{
	int tmp_id = pcb->managed_process[pcb->current_alloc_num].cpid = fork();

	if (tmp_id)
	{
		printf("I'm parent(process control manager) - (pid: %d), (cpid: %d)\n", getpid(), tmp_id);
		pcb->current_alloc_num++;
	}
	else if (!tmp_id)
	{
		int i;
		srand(time(NULL));
		strncpy(pcb->managed_process->name, "LTE Comm", 9);
		printf("I'm child(%s process) - (pid: %d), (cpid: %d)\n",
				pcb->managed_process->name, getpid(), tmp_id);

		sleep(3);

		for (i = 0; i < 10; i++)
		{
			printf("signal intensity: %d\n", rand() % 100);
		}

		exit(PROCESS_ALL_OK);
	}
}

void create_process(int req_num, process_control_block *pcb)
{
	switch (req_num)
	{
		case CAMERA:
			create_camera_process(pcb);
			break;

		case LIDAR:
			create_lidar_process(pcb);
			break;

		case CONTROL:
			create_control_process(pcb);
			break;

		case RADAR:
			create_radar_process(pcb);
			break;

		case LTE:
			create_lte_process(pcb);
			break;

		default:
			printf("잘못된 요청\n");
			break;
	}
}

int main(void)
{
	int request_number;
	char buf[SLAB_SIZE] = { 0 };
	int manager_pid = getpid();
	process_control_block *pcb = NULL;

	pcb = init_process_control_block();
	
	// Clean Architecture를 적용하지 않음
	// 예제 코드 답게 더티하게 구현함
	while (1)
	{
		int current_pid = getpid();

		if (manager_pid == current_pid)
		{
			if (pcb->current_alloc_num < 5)
			{
				printf("사용자 요청을 수신합니다\n");
				printf("카메라: 1, 라이다: 2, 제어: 3, 레이다: 4, LTE: 5\n");
			
				request_number = analysis_user_input(buf);
				create_process(request_number, pcb);
			}
		}
	}

	return 0;
}
