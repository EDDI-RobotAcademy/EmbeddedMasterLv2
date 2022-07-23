#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

#define PROCNUM 5

typedef enum _proc_num proc_num;
enum _proc_num{
	traffic_signal = 0,
	traffic_lamp,
	vehicle_lift,
	hydraulic_valve,
	hydraulic_pump
};

typedef struct _tc_pid tc_pid;
struct _tc_pid{
	pid_t traffic_signal;
	pid_t traffic_lamp;
	pid_t vehicle_lift;
	pid_t hydraulic_valve;
	pid_t hydraulic_pump;
};

void run_traffic_signal(void)
{
	printf("running traffic signal system\n");
}
void run_traffic_lamp(void)
{
	printf("running traffic lamp system\n");
}
void run_vehicle_lift(void)
{
	printf("running vehicle lift system\n");
}
void run_hydraulic_valve(void)
{
	printf("running hydraulic valve system\n");
}
void run_hydraulic_pump(void)
{
	printf("running hydraulic pump system\n");
}

void init_proc_type_intf(void (*(*fp_proc))(void))
{
	fp_proc[traffic_signal] = run_traffic_signal;
	fp_proc[traffic_lamp] = run_traffic_lamp;
	fp_proc[vehicle_lift] = run_vehicle_lift;
	fp_proc[hydraulic_valve] = run_hydraulic_valve;
	fp_proc[hydraulic_pump] = run_hydraulic_pump;
}

int main(int argc, char **argv)
{
	int i = 0;
	int num;
	pid_t pid[PROCNUM];
	bool p_flg = true;

	void (*fp_proc[PROCNUM])();
	void (*fp_run_proc)();

	init_proc_type_intf(fp_proc);

	while(p_flg)
	{
		printf("실행시킬 프로그램 번호를 입력하세요\n");
		printf("1. 신호등, 2. 가로등, 3. 리프트, 4. 밸브, 5. 펌프\n");
		scanf("%d", &num);
		pid[i] = fork();
		fp_run_proc = fp_proc[num-1];

		if(pid[i] < 0)
			exit(1);
		else if(pid[i])
			printf("p_pid : %d, c_pid : %d\n", getpid(), pid[i]);
		else
		{
			p_flg = false;
			//명령을 수신받고 자식 프로세스 실행은 2초 후에 실행 된다
			sleep(2);
			break;
		}
		i++;

		if(i > 4)
			break;
	}

	while(1)
	{
		if(!pid[num-1])
		{
			//COW(Copy On Write) 확인 용
			printf("num-1 = %d\n", num-1);
			printf("pid : %d, c_pid : %d\n", getpid(), pid[num-1]);
			fp_run_proc();
			sleep(1);
		}
	}

	return 0;
}
