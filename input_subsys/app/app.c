#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

#include<linux/input.h>
#include<linux/input-event-codes.h>

int main(int argc,char *argv[])
{
	int fd;
	int i = 10;

	struct input_event test_event;

	fd = open("/dev/input/event3",O_RDWR);

	if(fd < 0)
	{
		perror("open error\n");
		return fd;
	}
	
	while(i--)
	{
		read(fd,&test_event,sizeof(test_event));
		if(test_event.type==EV_KEY)
		{
			printf("type is %#x\n",test_event.type);
		}
	}	

	close(fd);
	return 0;
}
