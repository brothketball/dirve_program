#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	int fd;

	char rbuf[64] = {0};
	char wbuf[64] = "this data is from app";

	fd = open("/dev/hello_misc",O_RDWR);

	if(fd < 0)
	{
		perror("open error\n");
		return fd;
	}
	
	read(fd,rbuf,sizeof(rbuf));
	printf("buf is %s\n",rbuf);

	write(fd,wbuf,sizeof(wbuf));

	close(fd);
	return 0;
}
