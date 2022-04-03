#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
	int fd;

	//char rbuf[64] = {0};
	//char wbuf[64] = {0};
	int rbuf;

	/*if(argc != 2)
	{
		printf("error! Usage:<app> <参数>");
		return -1;
	}*/

	fd = open("/dev/simp_misc",O_RDWR);

	if(fd < 0)
	{
		perror("open error\n");
		return fd;
	}

	//wbuf[0] = atoi(argv[1]);//atoi用于将字符串转化为整型
	
	while(1)
	{
		read(fd,&rbuf,sizeof(rbuf));
		printf("buf is %d\n",rbuf);
	}
	

	//write(fd,wbuf,sizeof(wbuf));

	close(fd);
	return 0;
}
