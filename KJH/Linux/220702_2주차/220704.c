#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define ERROR 						-1

#define BUDDY_PAGE_SIZE				4096

// random number range : (0 + RANDOM_RANGE_OFFSET) ~ (RANDOM_RANGE + RANDOM_RANGE_OFFSET - 1)
#define RANDOM_RANGE				10
#define RANDOM_RANGE_OFFSET			0

#define COUNT						10


typedef struct _queue queue;
struct _queue
{
	int data;
	int freq;
	struct _queue *link;
};

queue *create_queue_node()
{
	queue *tmp;
	tmp = (queue *)malloc(sizeof(queue));
	tmp->link = NULL;
	return tmp;
}

void enqueue_data(queue **head, int data)
{
	if(!(*head))
	{
		*head = create_queue_node();
		(*head)->data = data;
		(*head)->freq = 1;
		return;
	}
	else
	{
		queue *backup = *head;
		queue *new = NULL;

		if(backup->data == data)
		{
			backup->freq++;
		}
		else
		{
			while(backup->link)
			{
				backup = backup->link;
				if(backup->data == data)
				{
					backup->freq++;
					return;
				}
			}
			new = create_queue_node();
			new->data = data;
			new->freq = 1;
			backup->link = new;
			return;
		}
	
		
	}
}

void print_queue(queue *head)
{
	printf("==================\n");
	while (head)
	{
		printf("queue = %d // freq = %d\n", head->data, head->freq);
		head = head->link;
	}
	printf("==================\n");
}

int return_random_value (void);
void file_write(int fd, char *buf, int buf_len);
void fd_error_check (int __fd);
void _error_handler (int error_code);

int main(int argc, char **argv)
{
	int src, dst, ret;
	char buf[BUDDY_PAGE_SIZE];
	char read_buf[BUDDY_PAGE_SIZE];
	int random;

	queue *head = NULL;

	if (argc < 2)
	{
		printf("사용법 : 복사할파일명 복사된파일명\n");
		exit(-1);
	}

	//random write
    for(int i = 1 ; i < argc ; i++)
	{
		src = open(argv[i], O_RDWR | O_CREAT | O_TRUNC, 0644);
		fd_error_check(src);

		for (int i = 0; i < COUNT; i++)
		{
			random = return_random_value();
			sprintf(buf, "%d\n", random);
			file_write(src, buf, strlen(buf));	
			enqueue_data(&head, random);
		}
		fd_error_check(src);

		close(src);
	}

	return 0;
}


int return_random_value (void)
{
	return (rand() % RANDOM_RANGE) + RANDOM_RANGE_OFFSET;
}


void file_write(int fd, char *buf, int buf_len)
{
	int write_result;

	buf_len = strlen(buf);
	write_result = write(fd, buf, buf_len);

	if (write_result != buf_len)
	{
		printf("write error!\n");
		exit(2);
	}
}

void fd_error_check (int __fd)
{
	if( __fd == ERROR)
	{
		_error_handler(ERROR);
	}
}

void _error_handler (int error_code)
{
	printf("error hanler : %d", error_code);
	exit(1);
}