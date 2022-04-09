#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#include<linux/input.h>

int main(int argc,char *argv[])
{
	int fd;

	struct input_event test_event;

	if(argc != 2)
	{
		printf("error! Usage:<app> <参数>");
		return -1;
	}

	fd = open("/dev/input/event3",O_RDWR);

	if(fd < 0)
	{
		perror("open error\n");
		return fd;
	}
	
	read(fd,&test_event,sizeof(test_event));
	printf("type is %#x\n",test_event.type);

	close(fd);
	return 0;
}
