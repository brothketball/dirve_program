#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>

#define CMD_TEST0 _IO('L',0)
#define CMD_TEST1 _IO('L',1)
#define CMD_TEST2 _IOW('L',2,int)
#define CMD_TEST3 _IOW('L',3,int)
#define CMD_TEST4 _IOR('L',4,int)

int main(int argc,char *argv[])
{
	int fd;
	int i;
	int value;
	
	fd = open("/dev/simp_misc",O_RDWR);

	if(fd < 0)
	{
		perror("open error\n");
		return fd;
	}

	for(i=0;i<6;i++)
	{
		ioctl(fd,CMD_TEST4,&value);
		sleep(2);
		printf("value is %d\n",value);
	}
	
	close(fd);
	return 0;
}
